#pragma once

#ifndef H_NETWORK_MANAGER_SERVER
#define H_NETWORK_MANAGER_SERVER

#include "Common\NetworkManager.h"

#define SEGMENT_SIZE 1500
#define SEND_INTERVAL 0.1f // How many times the server will send data to the players per second

class PacketStream;
class CPosition;
class CMovement;
class CNetworkManagerServer : public NetworkManager
{
public:
	CNetworkManagerServer();
	virtual ~CNetworkManagerServer();
	virtual bool Init(uint16_t aPort) override;
	
	void UpdateSendingSockets(float aDeltaTime);
	void UpdateReceivingSockets(float aDeltaTime);
	void UpdatePackets(float aDeltaTime);
private:
	bool InitServerListener();
	virtual bool InitReflection();
	void InitDataPos(const TCPSocketPtr& socket);
	void ProcessDataFromClientPos(CPosition* pos, float dt);
	void ManageDisconnection(TCPSocketPtr socket);
	std::vector<TCPSocketPtr> m_Sockets;
	std::vector<TCPSocketPtr> m_ReadSockets;
	std::vector<TCPSocketPtr> m_WriteSockets;
	std::vector<TCPSocketPtr> m_ErrorSockets;
	float m_SendTimer;
	std::map<TCPSocketPtr, CPosition*> m_Positions; // TEMP: In client proxy
	CMovement* m_Movement; // TEMP: In client proxy?
	std::map<TCPSocketPtr, PacketStream*> m_PacketStreams; // TEMP: PacketStreams in client proxy, this map should be <TCPSocketPtr, ClientProxy>
};

#endif