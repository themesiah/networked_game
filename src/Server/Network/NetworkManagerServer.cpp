#include "NetworkManagerServer.h"

#include "../ServerLogger.h"

#include "Serializer\PacketStream.h"
#include "Socket\SocketAddress.h"
#include "Replication\ObjectCreationRegistry.h"
#include "Replication\Packet.h"
#include "Replication\ReplicationManager.h"

#include "CommonClasses\Movement.h"
#include "../Model/Scenario/TilemapServer.h"

#include "../Server/ServerEngine/ServerEngine.h"
#include "ClientProxy.h"

CNetworkManagerServer::CNetworkManagerServer() : NetworkManager()
, m_SendTimer(0.f)
{
}

CNetworkManagerServer::~CNetworkManagerServer()
{
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
	SET_REFLECTION_DATA(TilemapServer);
	return true;
}

void CNetworkManagerServer::UpdateSendingSockets(float aDeltaTime)
{
	// Update the sending at different intervals
	m_SendTimer += aDeltaTime;

	if (m_SendTimer >= SEND_INTERVAL)
	{
		if (SocketUtil::Select(&m_Sockets, &m_ReadSockets, &m_Sockets, &m_WriteSockets, &m_Sockets, &m_ErrorSockets))
		{
			CReplicationManager& lReplicationManager = CServerEngine::GetInstance().GetReplicationManager();
			OutputMemoryBitStream lOutputDeltas;
			lReplicationManager.ReplicateWorldDeltas(lOutputDeltas, *CServerEngine::GetInstance().GetGameObjects());
			lOutputDeltas.Close();
			OutputMemoryBitStream lOutput;
			lReplicationManager.ReplicateWorldState(lOutput, *CServerEngine::GetInstance().GetGameObjects());
			lOutput.Close();
			for (const TCPSocketPtr& socket : m_WriteSockets)
			{
				CClientProxy* lClient = m_Clients[socket];
				if (lClient->GetState() == CClientProxy::ClientState::PLAYING) {
					socket->Send(lOutputDeltas.GetBufferPtr(), lOutputDeltas.GetByteLength());
				}
				else if (lClient->GetState() == CClientProxy::ClientState::AWAITING_GAME_STATE) {
					lClient->SetPlaying();
					OutputMemoryBitStream lOutputName;
					lOutputName.Serialize(PacketType::PT_Hello, PACKET_BIT_SIZE);
					lOutputName.Close();
					socket->Send(lOutputName.GetBufferPtr(), lOutputName.GetByteLength());
					socket->Send(lOutput.GetBufferPtr(), lOutput.GetByteLength());
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
				InputMemoryBitStream lInput(p.buffer, p.size);
				lInput.Serialize(packetType, PACKET_BIT_SIZE);
				CClientProxy* lClient = m_Clients[socket];
				if (packetType == PacketType::PT_ReplicationData && lClient->GetState() == CClientProxy::ClientState::PLAYING) {
					m_Clients[socket]->ProcessInput(aDeltaTime, lInput);
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