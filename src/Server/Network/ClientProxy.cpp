#include "ClientProxy.h"

#include "../ServerEngine/ServerEngine.h"

#include "Serializer\PacketStream.h"
#include "CommonClasses\Movement.h"

#include "Replication\ReplicationManager.h"
#include "Replication\LinkingContext.h"
#include "../Model/Player/PlayerControllerServer.h"
#include "../Network/RPCManagerServer.h"
#include "../Model/Scenario/CityMap.h"

CClientProxy::CClientProxy() :
m_Name("")
, m_PlayerController(NULL)
, m_PacketStream(NULL)
, m_State(ClientState::NOT_CONNECTED)
, m_Playername(NULL)
{
	m_CityMap = CServerEngine::GetInstance().GetCityMap();
}

CClientProxy::~CClientProxy()
{
	Disconnect();
}

bool CClientProxy::Init()
{
	m_PacketStream = new PacketStream();
	m_State = ClientState::CONNECTED;
	
	return true;
}

void CClientProxy::InitPlayer(InputMemoryBitStream& aInput)
{
	std::string lName;
	uint16_t lAnimationId;
	aInput.Serialize(lName);
	aInput.Serialize(lAnimationId); // TODO: Check if its a valid ID!

	auto lGameObjects = CServerEngine::GetInstance().GetGameObjects();
	m_PlayerController = new CPlayerControllerServer();
	m_PlayerController->Init(m_CityMap);
	m_PlayerController->SetAnimationId(lAnimationId);
	lGameObjects->push_back(m_PlayerController);

	LinkingContext* lLinkingContext = CServerEngine::GetInstance().GetReplicationManager().GetLinkingContext();
	m_Playername = new PlayernameServer();
	m_Playername->SetPlayer(lLinkingContext->GetNetworkId(m_PlayerController, true), lName);
	lGameObjects->push_back(m_Playername);
}

void CClientProxy::SetPlaying()
{
	m_State = ClientState::PLAYING;
}

void CClientProxy::SetWaiting()
{
	m_State = ClientState::AWAITING_GAME_STATE;
}

void CClientProxy::Disconnect()
{
	m_State = ClientState::PENDING_DISCONNECTION;
	auto lGameObjects = CServerEngine::GetInstance().GetGameObjects();
	if (m_PlayerController != NULL) {
		m_PlayerController->DestroySignal();
	}
	if (m_Playername != NULL) {
		m_Playername->DestroySignal();
	}
	delete m_PacketStream;
}

void CClientProxy::ProcessRPC(InputMemoryBitStream& aInput, float dt)
{
	CServerEngine::GetInstance().GetRPCManagerServer().ProcessPlayerRPC(aInput, this, dt);
}

void CClientProxy::Update(const float& dt)
{
	if (m_State == ClientState::PLAYING)
	{
		m_PlayerController->Update(dt);
	}
}