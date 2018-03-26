#include "ClientProxy.h"

#include "../ServerEngine/ServerEngine.h"

#include "Serializer\PacketStream.h"

CClientProxy::CClientProxy() :
m_Name("")
, m_Position(NULL)
, m_PacketStream(NULL)
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
	return true;
}

void CClientProxy::Disconnect()
{
	auto lGameObjects = CServerEngine::GetInstance().GetGameObjects();
	auto goit = std::find(lGameObjects->begin(), lGameObjects->end(), m_Position);
	lGameObjects->erase(goit);
	delete m_Position;
	delete m_PacketStream;
}