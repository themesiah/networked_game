#include "ServerEngine.h"

#include <iostream>
#include <assert.h>

#include "Socket\SocketAddressFactory.h"
#include "Replication\ObjectCreationRegistry.h"
#include "Replication\Packet.h"

#include "Replication\ReplicationManager.h"
#include "../Network/NetworkManagerServer.h"
#include "Movement.h"

CServerEngine::CServerEngine()
{
	
}

CServerEngine::~CServerEngine()
{
	delete m_ReplicationManager;
	delete m_NetworkManagerServer;

	for (std::vector<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		delete *it;
	}
}

void CServerEngine::Init()
{
	bool success = true;
	LOGGER.Info("Initializing server");
	LOGGER.Info("Initializing replication manager");
	CReplicationManager* lReplicationManager = new CReplicationManager();
	SetReplicationManager(lReplicationManager);
	LOGGER.Info("Initializing network manager");
	CNetworkManagerServer* lNetworkManager = new CNetworkManagerServer();
	success = lNetworkManager->Init(6900);
	SetNetworkManagerServer(lNetworkManager);
	m_PrevTime = m_Clock.now();
	LOGGER.Success("Server successfully initialized");
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

	m_NetworkManagerServer->UpdateSendingSockets(dt);
	m_NetworkManagerServer->UpdateReceivingSockets(dt);
	m_NetworkManagerServer->UpdatePackets(dt);	
}