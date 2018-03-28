#include "NetworkManager.h"

#include "../Socket/SocketUtil.h"
#include "../Socket/SocketAddress.h"

NetworkManager::NetworkManager() :
m_Socket(NULL)
{
}

NetworkManager::~NetworkManager()
{
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