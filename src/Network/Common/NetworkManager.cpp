#include "NetworkManager.h"

#include "../Socket/SocketUtil.h"
#include "../Socket/SocketAddress.h"

NetworkManager::NetworkManager() :
m_Socket(NULL)
{
	m_InputMs = new InputMemoryBitStream("a", 8);
	m_OutputMs = new OutputMemoryBitStream();
}

NetworkManager::~NetworkManager()
{
	delete m_InputMs;
	delete m_OutputMs;
}

bool NetworkManager::Init(uint16_t aPort)
{
	SocketUtil::InitSockets();
	m_Socket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress lReceivingAddress(INADDR_ANY, aPort);
	if (m_Socket->Bind(lReceivingAddress) != NO_ERROR)
	{
		return false;
	}	
	if (!InitReflection()) {
		return false;
	}
	return true;
}