#pragma once

#ifndef H_CLIENT_PROXY
#define H_CLIENT_PROXY

#include <string>

#include "Socket\TCPSocket.h"
#include "Replication\GameObject.h"

#include "Utils\Defines.h"

class CPosition : public GameObject {
public:
	CPosition() {
	}
	float posx;
	float posy;
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("posx", OffsetOf(CPosition, posx), -1000.0f, 0.1f),
			MemberVariable("posy", OffsetOf(CPosition, posy), -1000.0f, 0.1f)
		}
		);
	}
	CLASS_IDENTIFICATION('CPCT', CPosition);
};

class PacketStream;
class CClientProxy
{
public:
	CClientProxy();
	virtual ~CClientProxy();
	bool Init();
	void Disconnect();
	GET_SET(std::string, Name);
	PacketStream* GetPacketStream() {
		return m_PacketStream;
	}
	CPosition* GetPosition() {
		return m_Position;
	}
private:
	std::string m_Name;
	PacketStream* m_PacketStream;
	CPosition* m_Position;
};

#endif