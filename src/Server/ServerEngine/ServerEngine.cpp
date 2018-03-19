#include "ServerEngine.h"

#include <iostream>
#include <assert.h>

#include "Common\SocketAddressFactory.h"
#include "Serializer\OutputMemoryBitStream.h"
#include "Serializer\InputMemoryBitStream.h"
#include "Replication\ObjectCreationRegistry.h"
#include "Replication\Packet.h"

#include "Replication\ReplicationManager.h"
#include "Movement.h"

#include "Utils\Logger\Logger.h"

CServerEngine::CServerEngine() :
m_ListenSocket(NULL)
, m_SendTimer(0.f)
{
	m_InputMs = new InputMemoryBitStream("a", 8);
	m_OutputMs = new OutputMemoryBitStream();
	m_Movement = new CMovement();
}

CServerEngine::~CServerEngine()
{
	delete m_InputMs;
	delete m_OutputMs;
	delete m_ReplicationManager;
	delete m_Movement;

	for (std::map<TCPSocketPtr, PacketStream*>::iterator it = m_PacketStreams.begin(); it != m_PacketStreams.end(); ++it)
	{
		delete it->second;
	}

	for (std::vector<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		delete *it;
	}
}

void CServerEngine::Init()
{
	CReplicationManager* lReplicationManager = new CReplicationManager();
	SetReplicationManager(lReplicationManager);

	InitSockets();
	InitReflection();
	m_PrevTime = m_Clock.now();
}

void CServerEngine::InitSockets()
{
	SocketUtil::InitSockets();
	m_ListenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress lReceivingAddress(INADDR_ANY, 6900);
	if (m_ListenSocket->Bind(lReceivingAddress) != NO_ERROR)
	{
		LOG_ERROR_APPLICATION("Socket can't bind with error %d", WSAGetLastError());
		assert(false);
	}

	if (m_ListenSocket->Listen() != NO_ERROR)
	{
		LOG_ERROR_APPLICATION("Socket can't listen with error %d", WSAGetLastError());
		assert(false);
	}
	m_ListenSocket->SetNonBlockingMode(false);
	m_Sockets.push_back(m_ListenSocket);
}

void CServerEngine::InitReflection()
{
	SET_REFLECTION_DATA(CPosition);
	SET_REFLECTION_DATA(CMovement);
}

void CServerEngine::InitDataPos(const TCPSocketPtr& socket) {
	CPosition* l_Position = new CPosition();
	l_Position->posx = 100.f;
	l_Position->posy = 100.f;
	m_Positions[socket] = l_Position;
	m_GameObjects.push_back(l_Position);
}

void CServerEngine::ProcessDataFromClientPos(CPosition* pos, float dt)
{
	const float PLAYER_SPEED = 150.f;
	m_Movement->Serialize(m_InputMs);
	pos->posx += m_Movement->inputX * PLAYER_SPEED * dt;
	pos->posy += m_Movement->inputY * PLAYER_SPEED * dt;
	m_Movement->Reset();
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

	UpdateSendingSockets(dt);
	
	UpdateReceivingSockets(dt);

	UpdatePackets(dt);
	
}

void CServerEngine::UpdateSendingSockets(float aDeltaTime)
{
	// Update the sending at different intervals
	m_SendTimer += aDeltaTime;

	if (m_SendTimer >= SEND_INTERVAL)
	{
		m_OutputMs->Reset();
		m_ReplicationManager->ReplicateWorldState(m_OutputMs, m_GameObjects);
		m_OutputMs->WriteSize();
		if (SocketUtil::Select(&m_Sockets, &m_ReadSockets, &m_Sockets, &m_WriteSockets, &m_Sockets, &m_ErrorSockets))
		{
			for (const TCPSocketPtr& socket : m_WriteSockets)
			{
				socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
			}
		}
		m_SendTimer = fmod(m_SendTimer, SEND_INTERVAL);
	}
}

void CServerEngine::UpdateReceivingSockets(float aDeltaTime)
{
	if (SocketUtil::Select(&m_Sockets, &m_ReadSockets, &m_Sockets, &m_WriteSockets, &m_Sockets, &m_ErrorSockets))
	{
		for (const TCPSocketPtr& socket : m_ReadSockets)
		{
			if (socket == m_ListenSocket)
			{
				SocketAddress newClientAddress;
				auto newSocket = m_ListenSocket->Accept(newClientAddress);
				if (newSocket > 0) {
					m_Sockets.push_back(newSocket);
					m_PacketStreams[newSocket] = new PacketStream();
					std::cout << "New connection" << std::endl;
					InitDataPos(newSocket);
				}
			}
			else
			{
				char segment[SEGMENT_SIZE];
				FD_ZERO(segment);
				int dataReceived = socket->Receive(segment, SEGMENT_SIZE);
				if (dataReceived > 0)
				{
					m_PacketStreams[socket]->WriteBytes(segment, dataReceived);
				}
				if (dataReceived < 0 && WSAGetLastError() == WSAECONNRESET) {
					ManageDisconnection(socket);
				}
			}
		}
	}
}

void CServerEngine::UpdatePackets(float aDeltaTime)
{
	std::vector<TCPSocketPtr> tempSocketList = m_Sockets;
	for (const TCPSocketPtr& socket : tempSocketList)
	{
		if (socket != m_ListenSocket)
		{
			PacketStream::Packet p;
			p = m_PacketStreams[socket]->ReadPacket();
			while (p.size > 0) {
				// Process packet
				uint8_t packetType;
				m_InputMs->Reset(p.buffer, p.size);
				((MemoryStream*)m_InputMs)->Serialize(packetType, PACKET_BIT_SIZE);
				if (packetType == PacketType::PT_ReplicationData) {
					ProcessDataFromClientPos(m_Positions[socket], aDeltaTime);
				}
				else if (packetType == PacketType::PT_Disconnect) {
					ManageDisconnection(socket);
					std::free(p.buffer);
					break;
				}
				std::free(p.buffer);
				p = m_PacketStreams[socket]->ReadPacket();
			}
		}
	}
}

void CServerEngine::ManageDisconnection(TCPSocketPtr socket)
{
	std::cout << "Disconnected socket" << std::endl;
	auto it = std::find(m_Sockets.begin(), m_Sockets.end(), socket);
	m_Sockets.erase(it);
	auto goit = std::find(m_GameObjects.begin(), m_GameObjects.end(), m_Positions[socket]);
	m_GameObjects.erase(goit);
	delete m_Positions[socket];
	m_Positions.erase(socket);
	delete m_PacketStreams[socket];
	m_PacketStreams.erase(socket);
}