#pragma once

#ifndef H_CLIENT_PROXY
#define H_CLIENT_PROXY

#include <string>

#include "Socket\TCPSocket.h"
#include "Replication\GameObject.h"
#include "../Model/PlayernameServer.h"

#include "Utils\Defines.h"

class PacketStream;
class CPlayerControllerServer;
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
	void InitPlayer(InputMemoryBitStream& aInput);
	void Disconnect();
	void SetPlaying();
	void SetWaiting();
	void ProcessInput(float dt, InputMemoryBitStream& aInput);
	GET_SET(std::string, Name);
	PacketStream* GetPacketStream()
	{
		return m_PacketStream;
	}
	CPlayerControllerServer* GetPlayerController()
	{
		return m_PlayerController;
	}
	ClientState GetState()
	{
		return m_State;
	}
private:
	ClientState m_State;
	std::string m_Name;
	PacketStream* m_PacketStream;
	CPlayerControllerServer* m_PlayerController;;
	PlayernameServer* m_Playername;
};

#endif