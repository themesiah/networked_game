#pragma once

#ifndef H_CLIENT_PROXY
#define H_CLIENT_PROXY

#include <string>

#include "Socket\TCPSocket.h"
#include "Replication\GameObject.h"
#include "../Model/PlayernameServer.h"

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
	enum ClientState {
		NOT_CONNECTED = 0,
		CONNECTED,
		AWAITING_GAME_STATE, // After receiving HELLO from client, the server should send the full game state of the player location
		PLAYING,
		PENDING_DISCONNECTION
	};
	CClientProxy();
	virtual ~CClientProxy();
	bool Init();
	void SetName(InputMemoryBitStream& aInput);
	void Disconnect();
	void SetPlaying();
	void SetWaiting();
	void ProcessInput(float dt, InputMemoryBitStream& aInput);
	GET_SET(std::string, Name);
	PacketStream* GetPacketStream()
	{
		return m_PacketStream;
	}
	CPosition* GetPosition()
	{
		return m_Position;
	}
	ClientState GetState()
	{
		return m_State;
	}
private:
	ClientState m_State;
	std::string m_Name;
	PacketStream* m_PacketStream;
	CPosition* m_Position;
	PlayernameServer* m_Playername;
};

#endif