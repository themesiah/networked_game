#include "ServerEngine.h"

#include <iostream>
#include <assert.h>

#include "Common\SocketAddressFactory.h"
#include "Serializer\OutputMemoryBitStream.h"
#include "Serializer\InputMemoryBitStream.h"
#include "Replication\ObjectCreationRegistry.h"

#include "Replication\ReplicationManager.h"
#include "Movement.h"

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
	SocketAddress lReceivingAddress(INADDR_ANY, 48000);
	if (m_ListenSocket->Bind(lReceivingAddress) != NO_ERROR)
	{
		assert(false);
	}

	if (m_ListenSocket->Listen() != NO_ERROR)
	{
		assert(false);
	}
	m_ListenSocket->SetNonBlockingMode(true);
	m_ReadBlockSockets.push_back(m_ListenSocket);
	m_WriteBlockSockets.push_back(m_ListenSocket);
	m_ErrorBlockSockets.push_back(m_ListenSocket);
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

void CServerEngine::ProcessDataFromClientPos(char* segment, int dataReceived, CPosition* pos, float dt)
{
	const float PLAYER_SPEED = 1500.f;
	m_InputMs->Reset(segment, dataReceived);
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
	float dt = lChronoDeltaTime.count();// > 0.5f ? 0.5f : lChronoDeltaTime.count(); // DELTA TIME MAX 0.5f
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
		if (SocketUtil::Select(nullptr, nullptr,
			&m_WriteBlockSockets, &m_WritableSockets,
			nullptr, nullptr))
		{
			m_OutputMs->Reset();
			m_ReplicationManager->ReplicateWorldState(m_OutputMs, m_GameObjects);
			m_OutputMs->WriteSize();
			for (const TCPSocketPtr& socket : m_WritableSockets)
			{
				socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
			}
		}
		m_SendTimer = fmod(m_SendTimer, SEND_INTERVAL);
	}
}

void CServerEngine::UpdateReceivingSockets(float aDeltaTime)
{
	if (SocketUtil::Select(&m_ReadBlockSockets, &m_ReadableSockets,
		&m_WriteBlockSockets, &m_WritableSockets,
		&m_ErrorBlockSockets, &m_ErrorableSockets))
	{
		for (const TCPSocketPtr& socket : m_ErrorableSockets){}
		for (const TCPSocketPtr& socket : m_ReadableSockets)
		{
			if (socket == m_ListenSocket)
			{
				SocketAddress newClientAddress;
				auto newSocket = m_ListenSocket->Accept(newClientAddress);
				m_ReadBlockSockets.push_back(newSocket);
				m_WriteBlockSockets.push_back(newSocket);
				m_ErrorBlockSockets.push_back(newSocket);
				m_PacketStreams[newSocket] = new PacketStream();
				std::cout << "New connection" << std::endl;
				InitDataPos(newSocket);
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
					std::cout << "Disconnected socket" << std::endl;
					auto it = std::find(m_ReadBlockSockets.begin(), m_ReadBlockSockets.end(), socket);
					m_ReadBlockSockets.erase(it);
					it = std::find(m_WriteBlockSockets.begin(), m_WriteBlockSockets.end(), socket);
					m_WriteBlockSockets.erase(it);
					it = std::find(m_ErrorBlockSockets.begin(), m_ErrorBlockSockets.end(), socket);
					m_ErrorBlockSockets.erase(it);
					auto goit = std::find(m_GameObjects.begin(), m_GameObjects.end(), m_Positions[socket]);
					m_GameObjects.erase(goit);
					delete m_Positions[socket];
					m_Positions.erase(socket);
					delete m_PacketStreams[socket];
					m_PacketStreams.erase(socket);
				}
			}
		}
	}
}

void CServerEngine::UpdatePackets(float aDeltaTime)
{
	for (int i = 0; i < m_ReadBlockSockets.size(); ++i)
	{
		TCPSocketPtr socket = m_ReadBlockSockets[i];
		if (socket != m_ListenSocket)
		{
			PacketStream::Packet p;
			p = m_PacketStreams[socket]->ReadPacket();
			while (p.size > 0) {
				// Process packet
				ProcessDataFromClientPos(p.buffer, p.size, m_Positions[m_ReadBlockSockets[i]], aDeltaTime);
				std::free(p.buffer);
				p = m_PacketStreams[socket]->ReadPacket();
			}
		}
	}
}