#include "ServerEngine.h"

#include <iostream>
#include <assert.h>

#include "Socket\SocketAddressFactory.h"
#include "Replication\ObjectCreationRegistry.h"
#include "Replication\Packet.h"

#include "Replication\ReplicationManager.h"
#include "../Network/NetworkManagerServer.h"
#include "CommonClasses\Movement.h"
#include "../Network/RPCManagerServer.h"
#include "Replication\GameObject.h"
#include "../Model/Scenario/CityMap.h"
#include "Socket\SocketUtil.h"

#include "../ServerLogger.h"

CServerEngine::CServerEngine() :
m_Finished(false)
{
	
}

CServerEngine::~CServerEngine()
{
	delete m_ReplicationManager;
	delete m_NetworkManagerServer;
	delete m_RPCManagerServer;
	delete m_CityMap;
}

void CServerEngine::Init()
{
	bool success = true;
	LOGGER.Info("Initializing server");
	LOGGER.Info("Initializing RPC");
	CRPCManagerServer* lRPCManager = new CRPCManagerServer();
	SetRPCManagerServer(lRPCManager);
	BindRPCFunctionsServer<CServerEngine>(lRPCManager);
	BindRPCFunctionsServer<CClientProxy>(lRPCManager);
	BindRPCFunctionsServer<Place>(lRPCManager);
	LOGGER.Info("Initializing replication manager");
	CReplicationManager* lReplicationManager = new CReplicationManager();
	lReplicationManager->SetRPCManager(lRPCManager);
	SetReplicationManager(lReplicationManager);
	LOGGER.Info("Initializing network manager");
	CNetworkManagerServer* lNetworkManager = new CNetworkManagerServer();
	success = lNetworkManager->Init(6900);
	SetNetworkManagerServer(lNetworkManager);
	m_PrevTime = m_Clock.now();
	LOGGER.Success("Server successfully initialized");

	m_CityMap = new CityMap();
	m_CityMap->Load("ServerData/Cities/City1.json");
}

void CServerEngine::Update()
{
	// DELTA TIME CALCULATION BEGIN
	auto lCurrentTime = m_Clock.now();
	std::chrono::duration<float> lChronoDeltaTime = lCurrentTime - m_PrevTime;
	while (lChronoDeltaTime.count() < (1.f / TARGET_FPS)) { // FORCE MAX FPS
		lCurrentTime = m_Clock.now();
		lChronoDeltaTime = lCurrentTime - m_PrevTime;
	}
	m_PrevTime = lCurrentTime;
	float dt = lChronoDeltaTime.count();// > 0.5f ? 0.5f : lChronoDeltaTime.count();
	// DELTA TIME CALCULATION END

	m_CityMap->Update(dt);
	m_NetworkManagerServer->Update(dt);
	m_CityMap->ManageObjectsDestroy();
}

void CServerEngine::Shutdown() {
	m_Finished = true;
	m_NetworkManagerServer->Shutdown();
}