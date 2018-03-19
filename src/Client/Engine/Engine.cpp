#include "Engine.h"

#include <iostream>
#include <fstream>

#include "../Input/ActionManager.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "TextureManager.h"
#include "Replication\ObjectCreationRegistry.h"

#include "Common\SocketAddress.h"
#include "Common\SocketAddressFactory.h"

#include "Serializer\OutputMemoryBitStream.h"
#include "Serializer\InputMemoryBitStream.h"

#include "Replication\ReplicationManager.h"
#include "Movement.h"
#include "Replication\Packet.h"

#include "../Other/ImGUILog.h"

CEngine::CEngine() :
m_Socket(NULL)
, m_InputMs(NULL)
, m_OutputMs(NULL)
{
	m_Movement = new CMovement();
}

CEngine::~CEngine()
{
	m_InputMs->Reset("", 0);
	m_OutputMs->Reset();
	delete m_ActionManager;
	delete m_RenderManager;
	delete m_TextureManager;
	delete m_SpriteManager;
	delete m_ReplicationManager;
	delete m_Movement;
	delete m_InputMs;
	delete m_OutputMs;

	for (std::unordered_set<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		delete *it;
	}
	m_GameObjects.clear();
}

void CEngine::Init()
{
	CActionManager* lActionManager = new CActionManager();
	lActionManager->InitInputManager();
	lActionManager->LoadActions("actions.xml");
	SetActionManager(lActionManager);

	CTextureManager* lTextureManager = new CTextureManager();
	lTextureManager->SetOnDestructor(DestroyOnDestructor);
	SetTextureManager(lTextureManager);

	CSpriteManager* lSpriteManager = new CSpriteManager();
	lSpriteManager->SetOnDestructor(DestroyOnDestructor);
	SetSpriteManager(lSpriteManager);

	CRenderManager* lRenderManager = new CRenderManager();
	lRenderManager->Init();
	SetRenderManager(lRenderManager);

	CReplicationManager* lReplicationManager = new CReplicationManager();
	SetReplicationManager(lReplicationManager);

	InitReflection();
	InitNetwork();
}

void CEngine::InitReflection()
{
	SET_REFLECTION_DATA(CPlayerController);
	SET_REFLECTION_DATA(CMovement);
}

void CEngine::InitNetwork()
{
	IMLOG_INFO("Init network begin");
	m_InputMs = new InputMemoryBitStream("a", 8);
	m_OutputMs = new OutputMemoryBitStream();

	SocketUtil::InitSockets();
	m_Socket = SocketUtil::CreateTCPSocket(INET);
	std::string lAddressString = "localhost:48001";
	std::ifstream file("Data/server.txt");
	if (file.is_open())
	{
		std::getline(file, lAddressString);
		file.close();
	}
	SocketAddressPtr sendingAddress = SocketAddressFactory::CreateIPv4FromString(lAddressString);
	SocketAddress receivingAddress(INADDR_ANY, 0);
	if (m_Socket->Bind(receivingAddress) == NO_ERROR)
	{
		if (m_Socket->Connect(*sendingAddress.get()) == NO_ERROR)
		{
			m_Socket->SetNonBlockingMode(true);
		}
		else {
			LOG_ERROR_APPLICATION("Socket can't connect with error %d", WSAGetLastError());
			std::cout << "Socket can't connect: " << WSAGetLastError() << std::endl;
		}
	}
	else {
		LOG_ERROR_APPLICATION("Socket can't bind with error %d", WSAGetLastError());
		std::cout << "Socket can't bind: " << WSAGetLastError() << std::endl;
	}
	IMLOG_INFO("Init network end");
}

void CEngine::ProcessInputs()
{
	m_ActionManager->Update();
}

void CEngine::Update(float aDeltaTime)
{
	// MANAGE INPUT
	if (CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->active) {
		m_Movement->inputX = CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->value;
	}
	else if (CEngine::GetInstance().GetActionManager().Get("VERTICAL")->active) {
		m_Movement->inputY = CEngine::GetInstance().GetActionManager().Get("VERTICAL")->value;
	}
	// END MANAGE INPUT

	UpdateNetwork(aDeltaTime);

	for (GameObject* go : m_GameObjects)
	{
		go->Update(aDeltaTime);
	}
}

void CEngine::UpdateNetwork(float aDeltaTime)
{
	// SEND
	m_OutputMs->Reset();
	uint8_t packetType = PacketType::PT_ReplicationData;
	((MemoryStream*)m_OutputMs)->Serialize(packetType, PACKET_BIT_SIZE);
	m_Movement->Serialize(m_OutputMs);
	m_OutputMs->WriteSize();
	int sent = m_Socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
	m_Movement->Reset();

	// RECEIVE
	char segment[SEGMENT_SIZE];
	FD_ZERO(segment);
	int dataReceived = m_Socket->Receive(segment, SEGMENT_SIZE);
	if (dataReceived > 0) {
		m_PacketStream.WriteBytes(segment, dataReceived);
	}

	// PROCESS
	PacketStream::Packet p;
	p = m_PacketStream.ReadPacket();
	while (p.size > 0)
	{
		m_InputMs->Reset(p.buffer, p.size);
		uint8_t packetType;
		((MemoryStream*)m_InputMs)->Serialize(packetType, PACKET_BIT_SIZE);
		if (packetType == PacketType::PT_ReplicationData) {
			m_GameObjects = m_ReplicationManager->ReceiveReplicatedObjects(m_InputMs);
		}
		std::free(p.buffer);
		p = m_PacketStream.ReadPacket();
	}
}

void CEngine::Render(sf::RenderWindow* window)
{
	m_RenderManager->Render(window);
}

void CEngine::ShowDebugHelpers()
{
	IMLOG_DRAW;
	ImGui::Begin("Sample window"); // begin window
	ImGui::Text("FPS: %d", (int)ImGui::GetIO().Framerate);
	if (ImGui::Button("Hello, im a button")) {
	}
	ImGui::End();
}

void CEngine::Disconnect()
{
	m_OutputMs->Reset();
	uint8_t packetType = PacketType::PT_Disconnect;
	((MemoryStream*)m_OutputMs)->Serialize(packetType, PACKET_BIT_SIZE);
	m_OutputMs->WriteSize();
	m_Socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
}