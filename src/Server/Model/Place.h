#pragma once

#ifndef H_PLACE
#define H_PLACE

#include <vector>

#include "Serializer\InputMemoryBitStream.h"
#include "Serializer\OutputMemoryBitStream.h"


class CClientProxy;
class GameObject;
class Place
{
public:
	Place();
	virtual ~Place();
	void ManageChatRPC(InputMemoryBitStream& aInput, CClientProxy* aClientProxy);
	void ProcessChatMessages();
	void RegisterClient(CClientProxy* aClient);
	void UnregisterClient(CClientProxy* aClient);
	std::vector<CClientProxy*> GetClients()
	{
		return m_Clients;
	}
	OutputMemoryBitStream& GetChatOutput()
	{
		return m_ChatOutput;
	}
	void ManageObjectsDestroy();
	virtual void Update(const float& dt) = 0;
	virtual void OnClientRegister(CClientProxy* aClient) = 0;
	virtual void OnClientUnregister(CClientProxy* aClient) = 0;
	virtual void ProcessPlace();
	virtual OutputMemoryBitStream& GetOutput() = 0;
	virtual OutputMemoryBitStream& GetDeltasOutput() = 0;
	virtual bool IsCity() = 0;
	virtual bool Reload() = 0;
	virtual bool Load(const std::string& path) = 0;
protected:
	std::vector<CClientProxy*> m_Clients;
	std::vector<GameObject*> m_MapObjects;
private:
	std::vector<std::string> m_MessageCache;
	std::vector<std::string> m_NameCache;
	OutputMemoryBitStream m_ChatOutput;
};

#endif