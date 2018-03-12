#include "Engine.h"

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

CEngine::CEngine() :
m_Socket(NULL)
, m_InputMs(NULL)
, m_OutputMs(NULL)
{
	m_PlayerController = new CPlayerController();
	m_Movement = new CMovement();
}

CEngine::~CEngine()
{
	m_InputMs->Reset("", 0);
	m_OutputMs->Reset();
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

	m_PlayerController->Init();
}

void CEngine::InitReflection()
{
	SET_REFLECTION_DATA(CPlayerController);
	SET_REFLECTION_DATA(CMovement);
}

void CEngine::InitNetwork()
{
	m_InputMs = new InputMemoryBitStream("a", 8);
	m_OutputMs = new OutputMemoryBitStream();

	SocketUtil::InitSockets();
	m_Socket = SocketUtil::CreateTCPSocket(INET);
	SocketAddressPtr sendingAddress = SocketAddressFactory::CreateIPv4FromString("localhost:48000");
	SocketAddress receivingAddress(INADDR_ANY, 0);
	if (m_Socket->Bind(receivingAddress) == NO_ERROR)
	{
		if (m_Socket->Connect(*sendingAddress.get()) == NO_ERROR)
		{
			m_Socket->SetNonBlockingMode(true);
		}
	}
	m_ReadBlockSockets.push_back(m_Socket);
}

void CEngine::ProcessInputs()
{
	m_ActionManager->Update();
}

void CEngine::Update(float aDeltaTime)
{
	m_PlayerController->Update(aDeltaTime);

	/*for (GameObject* go : m_GameObjects)
	{
		go->Update(aDeltaTime);
	}*/

	// MANAGE INPUT
	if (CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->active) {
		m_Movement->inputX = CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->value;
	}
	else if (CEngine::GetInstance().GetActionManager().Get("VERTICAL")->active) {
		m_Movement->inputY = CEngine::GetInstance().GetActionManager().Get("VERTICAL")->value;
	}
	// END MANAGE INPUT

	UpdateNetwork(aDeltaTime);
}

void CEngine::UpdateNetwork(float aDeltaTime)
{
	m_OutputMs->Reset();
	m_Movement->Serialize(m_OutputMs);
	int sent = m_Socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
	m_Movement->Reset();

	std::vector<TCPSocketPtr> readableSockets;
	if (SocketUtil::Select(&m_ReadBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr)) {
		for (const TCPSocketPtr& socket : readableSockets)
		{
			char segment[1024];
			FD_ZERO(segment);
			int dataReceived = socket->Receive(segment, 1024);
			if (dataReceived > 0) {
				m_InputMs->Reset(segment, dataReceived);
				m_PlayerController->Serialize(m_InputMs);
			}
		}
	}
}

void CEngine::Render(sf::RenderWindow* window)
{
	m_RenderManager->Render(window);
}

void CEngine::ShowDebugHelpers()
{
	ImGui::Begin("Sample window"); // begin window
	if (ImGui::Button("Hello, im a button")) {

	}
	ImGui::End();
}