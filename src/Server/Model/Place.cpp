#include "Place.h"

#include "../Network/ClientProxy.h"
#include "Replication\Packet.h"

Place::Place()
{

}

Place::~Place()
{

}

void Place::ManageChatRPC(InputMemoryBitStream& aInput, CClientProxy* aClientProxy)
{
	std::string lName = aClientProxy->GetName();
	std::string lMessage;
	aInput.Serialize(lMessage);
	m_NameCache.push_back(lName);
	m_MessageCache.push_back(lMessage);
}

void Place::ProcessChatMessages()
{
	if (m_MessageCache.size() > 0)
	{
		m_ChatOutput.Serialize(PT_RPC, PACKET_BIT_SIZE);
		m_ChatOutput.Serialize<uint32_t>('CHAT');
		m_ChatOutput.Serialize<int>(m_MessageCache.size());
		for (int i = 0; i < m_MessageCache.size(); ++i)
		{
			m_ChatOutput.Serialize(m_NameCache[i]);
			m_ChatOutput.Serialize(m_MessageCache[i]);
		}
		m_ChatOutput.Close();
		m_MessageCache.clear();
		m_NameCache.clear();
	}
}

void Place::Reset()
{
	m_ChatOutput.Reset();
}

void Place::RegisterClient(CClientProxy* aClient)
{
	auto it = std::find(m_Clients.begin(), m_Clients.end(), aClient);
	if (it == m_Clients.end())
	{
		m_Clients.push_back(aClient);
	}
}

void Place::UnregisterClient(CClientProxy* aClient)
{
	auto it = std::find(m_Clients.begin(), m_Clients.end(), aClient);
	if (it != m_Clients.end())
	{
		m_Clients.erase(it);
	}
}