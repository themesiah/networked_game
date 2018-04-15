#pragma once

#ifndef H_SOCKET_ADDRESS
#define H_SOCKET_ADDRESS

#include <WinSock2.h>
#include <stdint.h>
#include <memory>

class SocketAddress
{
public:
	SocketAddress();
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& inSockAddr);
	size_t GetSize() const;
private:
	friend class UDPSocket;
	friend class TCPSocket;
	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn();

};
typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

#endif