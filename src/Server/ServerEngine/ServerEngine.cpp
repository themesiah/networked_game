#include "ServerEngine.h"

#include <iostream>
#include <assert.h>

#include "Common\SocketAddressFactory.h"
#include "Serializer\OutputMemoryBitStream.h"
#include "Serializer\InputMemoryBitStream.h"

CServerEngine::CServerEngine() :
m_ListenSocket(NULL)
, m_SendTimer(0.f)
{
	m_InputMs = new InputMemoryBitStream("a", 8);
	m_OutputMs = new OutputMemoryBitStream();
}

CServerEngine::~CServerEngine()
{
	delete m_InputMs;
	delete m_OutputMs;
}

void CServerEngine::Init()
{
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
}

void CServerEngine::InitDataPos(const TCPSocketPtr& socket) {
	CPosition* l_Position = new CPosition();
	l_Position->posx = 100.f;
	l_Position->posy = 100.f;
	m_Positions.push_back(l_Position);
}

void CServerEngine::ProcessDataFromClientPos(char* segment, int dataReceived, CPosition* pos)
{
	m_InputMs->Reset(segment, dataReceived);
	pos->Serialize(m_InputMs);
}

void CServerEngine::SendDataToClient(const TCPSocketPtr& socket, CPosition* pos)
{
	m_OutputMs->Reset();
	pos->Serialize(m_OutputMs);
	socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
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
					auto it = std::find(m_ReadBlockSockets.begin(), m_ReadBlockSockets.end(), socket) - m_ReadBlockSockets.begin() - 1;
					ProcessDataFromClientPos(segment, dataReceived, m_Positions[it]);
				}
				if (dataReceived < 0 && WSAGetLastError() == WSAECONNRESET) {
					std::cout << "Disconnected socket" << std::endl;
					auto it = std::find(m_ReadBlockSockets.begin(), m_ReadBlockSockets.end(), socket);
					m_ReadBlockSockets.erase(it);
					it = std::find(m_WriteBlockSockets.begin(), m_WriteBlockSockets.end(), socket);
					m_WriteBlockSockets.erase(it);
					it = std::find(m_ErrorBlockSockets.begin(), m_ErrorBlockSockets.end(), socket);
					m_ErrorBlockSockets.erase(it);
				}
			}
		}
	}
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
			for (const TCPSocketPtr& socket : m_WritableSockets)
			{
				auto it = std::find(m_WriteBlockSockets.begin(), m_WriteBlockSockets.end(), socket) - m_WriteBlockSockets.begin() - 1;
				SendDataToClient(socket, m_Positions[it]);
			}
		}
		m_SendTimer = fmod(m_SendTimer, SEND_INTERVAL);
	}
}