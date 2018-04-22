#pragma once

#ifndef H_SERVER_ENGINE
#define H_SERVER_ENGINE

#include <vector>
#include <chrono>

#include "Utils\Singleton.h"

#define TARGET_FPS 60

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
const C##Manager& Get##Manager() const { return *m_##Manager; } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Has##Manager() { return m_##Manager != nullptr; } \


class CReplicationManager;
class CNetworkManagerServer;
class CMovement;
class CRPCManagerServer;
class GameObject;
class Place;
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
	bool IsFinished() {
		return m_Finished;
	}
	Place* GetCityMap()
	{
		return m_CityMap;
	}
	void Shutdown();
	BUILD_GET_SET_ENGINE_MANAGER(ReplicationManager);
	BUILD_GET_SET_ENGINE_MANAGER(NetworkManagerServer);
	BUILD_GET_SET_ENGINE_MANAGER(RPCManagerServer);
protected:
	CServerEngine();
	friend class base::utils::CSingleton<CServerEngine>;
private:
	void ManageObjectsDestroy();
	std::chrono::monotonic_clock m_Clock;
	std::chrono::monotonic_clock::time_point m_PrevTime;
	std::vector<GameObject*> m_GameObjects;
	bool m_Finished;
	Place* m_CityMap;
};

#endif