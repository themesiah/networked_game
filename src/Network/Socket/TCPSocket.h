#pragma once

#ifndef H_TCP_SOCKET
#define H_TCP_SOCKET

#include <memory>
#include <WinSock2.h>

class SocketAddress;
class TCPSocket
{
public:
	~TCPSocket();
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBackLog = 32);
	std::shared_ptr< TCPSocket > Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);
	int SetNonBlockingMode(bool inShouldBeNonBlocking);
private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

#endif