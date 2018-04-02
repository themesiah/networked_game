#include "ClientProxy.h"

#include "../ServerEngine/ServerEngine.h"

#include "Serializer\PacketStream.h"
#include "Movement.h"

CClientProxy::CClientProxy() :
m_Name("")
, m_Position(NULL)
, m_PacketStream(NULL)
, m_State(ClientState::NOT_CONNECTED)
{

}

CClientProxy::~CClientProxy()
{
	Disconnect();
}

bool CClientProxy::Init()
{
	auto lGameObjects = CServerEngine::GetInstance().GetGameObjects();
	m_Position = new CPosition();
	m_Position->posx = 100.f;
	m_Position->posy = 100.f;
	lGameObjects->push_back(m_Position);

	m_PacketStream = new PacketStream();
	m_State = ClientState::CONNECTED;
	return true;
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
	auto goit = std::find(lGameObjects->begin(), lGameObjects->end(), m_Position);
	lGameObjects->erase(goit);
	delete m_Position;
	delete m_PacketStream;
}

void CClientProxy::ProcessInput(float dt, InputMemoryBitStream& aInput)
{
	const float PLAYER_SPEED = 150.f;
	CMovement lMovement;
	lMovement.SerializeRead(aInput);
	auto pos = lMovement.GetMovement();
	m_Position->posx += pos[0] * PLAYER_SPEED * dt;
	m_Position->posy += pos[1] * PLAYER_SPEED * dt;
	if (pos[0] != 0.f || pos[1] != 0.f)
	{
		m_Position->SetDirty();
	}
}