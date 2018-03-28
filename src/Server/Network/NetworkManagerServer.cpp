#include "NetworkManagerServer.h"

#include "../ServerLogger.h"

#include "Serializer\PacketStream.h"
#include "Socket\SocketAddress.h"
#include "Replication\ObjectCreationRegistry.h"
#include "Replication\Packet.h"
#include "Replication\ReplicationManager.h"

#include "Movement.h"
#include "../Server/ServerEngine/ServerEngine.h"
#include "ClientProxy.h"

CNetworkManagerServer::CNetworkManagerServer() : NetworkManager()
, m_SendTimer(0.f)
{
	m_Movement = new CMovement();
}

CNetworkManagerServer::~CNetworkManagerServer()
{
	delete m_Movement;
}

bool CNetworkManagerServer::Init(uint16_t aPort)
{
	if (!NetworkManager::Init(aPort))
		return false;
	m_Socket->SetNonBlockingMode(false);
	if (m_Socket->Listen() != NO_ERROR)
	{
		return false;
	}
	if (!InitServerListener())
		return false;
	return true;
}

bool CNetworkManagerServer::InitServerListener()
{
	m_Sockets.push_back(m_Socket);
	return true;
}

bool CNetworkManagerServer::InitReflection()
{
	LOGGER.Log("Setting reflection data of networked classes");
	SET_REFLECTION_DATA(CPosition);
	SET_REFLECTION_DATA(CMovement);
	return true;
}

void CNetworkManagerServer::UpdateSendingSockets(float aDeltaTime)
{
	// Update the sending at different intervals
	m_SendTimer += aDeltaTime;

	if (m_SendTimer >= SEND_INTERVAL)
	{
		CReplicationManager& lReplicationManager = CServerEngine::GetInstance().GetReplicationManager();
		OutputMemoryBitStream l_Output;
		lReplicationManager.ReplicateWorldState(l_Output, *CServerEngine::GetInstance().GetGameObjects());
		l_Output.WriteSize();
		if (SocketUtil::Select(&m_Sockets, &m_ReadSockets, &m_Sockets, &m_WriteSockets, &m_Sockets, &m_ErrorSockets))
		{
			for (const TCPSocketPtr& socket : m_WriteSockets)
			{
				CClientProxy* lClient = m_Clients[socket];
				if (lClient->GetState() == CClientProxy::ClientState::PLAYING) {
					socket->Send(l_Output.GetBufferPtr(), l_Output.GetByteLength());
				}
				else if (lClient->GetState() == CClientProxy::ClientState::AWAITING_GAME_STATE) {
					OutputMemoryBitStream l_OutHello;
					l_OutHello.Serialize(PT_Hello, PACKET_BIT_SIZE);
					l_OutHello.WriteSize();
					lClient->SetPlaying();
					socket->Send(l_OutHello.GetBufferPtr(), l_OutHello.GetByteLength());
				}
			}
		}
		m_SendTimer = fmod(m_SendTimer, SEND_INTERVAL);
	}
}

void CNetworkManagerServer::UpdateReceivingSockets(float aDeltaTime)
{
	if (SocketUtil::Select(&m_Sockets, &m_ReadSockets, &m_Sockets, &m_WriteSockets, &m_Sockets, &m_ErrorSockets))
	{
		for (const TCPSocketPtr& socket : m_ReadSockets)
		{
			if (socket == m_Socket)
			{
				ManageNewConnection();
			}
			else
			{
				char segment[SEGMENT_SIZE];
				FD_ZERO(segment);
				int dataReceived = socket->Receive(segment, SEGMENT_SIZE);
				if (dataReceived > 0)
				{
					m_Clients[socket]->GetPacketStream()->WriteBytes(segment, dataReceived);
				}
				if (dataReceived < 0 && WSAGetLastError() == WSAECONNRESET) {
					LOGGER.Warning("Socket disconnected forcefully");
					ManageDisconnection(socket);
				}
			}
		}
	}
}

void CNetworkManagerServer::UpdatePackets(float aDeltaTime)
{
	std::vector<TCPSocketPtr> tempSocketList = m_Sockets;
	for (const TCPSocketPtr& socket : tempSocketList)
	{
		if (socket != m_Socket)
		{
			PacketStream::Packet p;
			p = m_Clients[socket]->GetPacketStream()->ReadPacket();
			while (p.size > 0) {
				// Process packet
				uint8_t packetType;
				InputMemoryBitStream l_Input(p.buffer, p.size);
				l_Input.Serialize(packetType, PACKET_BIT_SIZE);
				CClientProxy* lClient = m_Clients[socket];
				if (packetType == PacketType::PT_ReplicationData && lClient->GetState() == CClientProxy::ClientState::PLAYING) {
					ProcessDataFromClientPos(lClient->GetPosition(), aDeltaTime, l_Input);
				}
				else if (packetType == PacketType::PT_Disconnect && lClient->GetState() != CClientProxy::ClientState::PENDING_DISCONNECTION) {
					ManageDisconnection(socket);
					std::free(p.buffer);
					break;
				}
				else if (packetType == PacketType::PT_Hello && lClient->GetState() == CClientProxy::ClientState::CONNECTED) {
					lClient->SetWaiting();
				}
				std::free(p.buffer);
				p = m_Clients[socket]->GetPacketStream()->ReadPacket();
			}
		}
	}
}

void CNetworkManagerServer::ManageDisconnection(TCPSocketPtr socket)
{
	LOGGER.Info("Socket disconnected");

	delete m_Clients[socket];
	m_Clients.erase(socket);
	
	auto it = std::find(m_Sockets.begin(), m_Sockets.end(), socket);
	m_Sockets.erase(it);
}

void CNetworkManagerServer::ManageNewConnection()
{
	SocketAddress newClientAddress;
	auto newSocket = m_Socket->Accept(newClientAddress);
	if (newSocket > 0) {
		m_Sockets.push_back(newSocket);
		CClientProxy* lClientProxy = new CClientProxy();
		lClientProxy->Init();
		m_Clients[newSocket] = lClientProxy;
		LOGGER.Info("New connection received");
	}
}

void CNetworkManagerServer::ProcessDataFromClientPos(CPosition* pos, float dt, InputMemoryBitStream& aInput)
{
	const float PLAYER_SPEED = 150.f;
	m_Movement->SerializeRead(aInput);
	pos->posx += m_Movement->inputX * PLAYER_SPEED * dt;
	pos->posy += m_Movement->inputY * PLAYER_SPEED * dt;
	m_Movement->Reset();
}