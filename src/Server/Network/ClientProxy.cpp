#include "ClientProxy.h"

#include "../ServerEngine/ServerEngine.h"

#include "Serializer\PacketStream.h"
#include "CommonClasses\Movement.h"

#include "Replication\ReplicationManager.h"
#include "Replication\GameObject.h"
#include "Replication\LinkingContext.h"

#include "../Network/RPCManagerServer.h"
#include "../Model/Scenario/CityMap.h"
#include "../Model/PlayernameServer.h"

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
	uint16_t lAnimationId;
	aInput.Serialize(m_Name);
	aInput.Serialize(lAnimationId); // TODO: Check if its a valid ID!

	//auto lGameObjects = CServerEngine::GetInstance().GetGameObjects();
	m_PlayerController = new CPlayerControllerServer();
	m_PlayerController->Init((CityMap*)m_CityMap);
	m_PlayerController->SetAnimationId(lAnimationId);
	//lGameObjects->push_back(m_PlayerController);

	LinkingContext* lLinkingContext = CServerEngine::GetInstance().GetReplicationManager().GetLinkingContext();
	m_Playername = new PlayernameServer();
	m_Playername->SetPlayer(lLinkingContext->GetNetworkId(m_PlayerController, true), m_Name);
	//lGameObjects->push_back(m_Playername);
	m_CityMap->RegisterClient(this);
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
	m_CityMap->UnregisterClient(this);
	delete m_PacketStream;
}

void CClientProxy::ProcessRPC(InputMemoryBitStream& aInput, float dt)
{
	CServerEngine::GetInstance().GetRPCManagerServer().ProcessPlayerRPC(aInput, this, dt);
}

PacketStream* CClientProxy::GetPacketStream()
{
	return m_PacketStream;
}
CPlayerControllerServer* CClientProxy::GetPlayerController()
{
	return m_PlayerController;
}
PlayernameServer* CClientProxy::GetPlayernameServer()
{
	return m_Playername;
}
CClientProxy::ClientState CClientProxy::GetState()
{
	return m_State;
}
Place* CClientProxy::GetPlace()
{
	return m_CityMap;
}