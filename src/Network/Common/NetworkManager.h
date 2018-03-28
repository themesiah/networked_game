#pragma once

#ifndef H_NETWORK_MANAGER
#define H_NETWORK_MANAGER

#include "../Socket/TCPSocket.h"
#include "../Serializer/InputMemoryBitStream.h"
#include "../Serializer/OutputMemoryBitStream.h"

class NetworkManager {
public:
	NetworkManager();
	virtual ~NetworkManager();
	virtual bool Init(uint16_t aPort);
protected:
	TCPSocketPtr m_Socket;
private:
	virtual bool InitReflection() = 0;
};

#endif