#pragma once

#ifndef H_NETWORK_MANAGER_SERVER
#define H_NETWORK_MANAGER_SERVER

#include "Common\NetworkManager.h"

#define SEGMENT_SIZE 1500
#define SEND_INTERVAL 0.1f // How many times the server will send data to the players per second

class CMovement;
class CClientProxy;
class CPosition;
class CNetworkManagerServer : public NetworkManager
{
public:
	CNetworkManagerServer();
	virtual ~CNetworkManagerServer();
	virtual bool Init(uint16_t aPort) override;
	
	void UpdateSendingSockets(float aDeltaTime);
	void UpdateReceivingSockets(float aDeltaTime);
	void UpdatePackets(float aDeltaTime);
	void Shutdown();
private:
	bool InitServerListener();
	virtual bool InitReflection();
	
	void ManageDisconnection(TCPSocketPtr socket);
	void ManageNewConnection();
	std::vector<TCPSocketPtr> m_Sockets;
	std::vector<TCPSocketPtr> m_ReadSockets;
	std::vector<TCPSocketPtr> m_WriteSockets;
	std::vector<TCPSocketPtr> m_ErrorSockets;
	float m_SendTimer;
	std::map<TCPSocketPtr, CClientProxy*> m_Clients;
	bool m_Closing;
};

#endif