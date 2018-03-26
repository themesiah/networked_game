#pragma once

#ifndef H_SERVER_ENGINE
#define H_SERVER_ENGINE

#include <vector>
#include <chrono>

#include "Utils\Singleton.h"
#include "Socket\SocketUtil.h"
#include "Replication\GameObject.h"
#include "../ServerLogger.h"

#define TARGET_FPS 60

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
const C##Manager& Get##Manager() const { return *m_##Manager; } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Has##Manager() { return m_##Manager != nullptr; } \

/*class OutputMemoryBitStream;
class InputMemoryBitStream;*/


class CReplicationManager;
class CNetworkManagerServer;
class CMovement;
class CServerEngine : public base::utils::CSingleton<CServerEngine>
{
public:
	virtual ~CServerEngine();
	void Init();
	void Update();
	std::vector<GameObject*>* GetGameObjects()
	{
		return &m_GameObjects;
	}
	BUILD_GET_SET_ENGINE_MANAGER(ReplicationManager);
	BUILD_GET_SET_ENGINE_MANAGER(NetworkManagerServer);
protected:
	CServerEngine();
	friend class base::utils::CSingleton<CServerEngine>;
private:
	std::chrono::monotonic_clock m_Clock;
	std::chrono::monotonic_clock::time_point m_PrevTime;
	
	std::vector<GameObject*> m_GameObjects; // TEMP: Make container with add, erase and a double buffer
};

#endif