#pragma once

#ifndef H_PLACE
#define H_PLACE

#include <vector>

#include "Serializer\InputMemoryBitStream.h"
#include "Serializer\OutputMemoryBitStream.h"


class CClientProxy;
class Place
{
public:
	
	Place();
	virtual ~Place();
	void ManageChatRPC(InputMemoryBitStream& aInput, CClientProxy* aClientProxy);
	void ProcessChatMessages();
	void Reset();
	void RegisterClient(CClientProxy* aClient);
	void UnregisterClient(CClientProxy* aClient);
	std::vector<CClientProxy*> GetClients()
	{
		return m_Clients;
	}
	OutputMemoryBitStream& GetOutput()
	{
		return m_ChatOutput;
	}
	virtual bool IsCity() = 0;
	virtual bool Reload() = 0;
	virtual bool Load(const std::string& path) = 0;
protected:
	std::vector<CClientProxy*> m_Clients;

private:
	std::vector<std::string> m_MessageCache;
	std::vector<std::string> m_NameCache;
	OutputMemoryBitStream m_ChatOutput;
};

#endif