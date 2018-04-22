#pragma once

#ifndef H_CLIENT_PROXY
#define H_CLIENT_PROXY

#include <string>

#include "Socket\TCPSocket.h"
#include "../Model/PlayernameServer.h"
#include "../Model/Place.h"
#include "../Model/Player/PlayerControllerServer.h"

#include "Utils\Defines.h"

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
	void InitPlayer(InputMemoryBitStream& aInput);
	void Disconnect();
	void SetPlaying();
	void SetWaiting();
	void ProcessRPC(InputMemoryBitStream& aInput, float dt);
	GET_SET(std::string, Name);
	PacketStream* GetPacketStream();
	CPlayerControllerServer* GetPlayerController();
	PlayernameServer* GetPlayernameServer();
	ClientState GetState();
	Place* GetPlace();
private:
	ClientState m_State;
	std::string m_Name;
	PacketStream* m_PacketStream;
	CPlayerControllerServer* m_PlayerController;;
	PlayernameServer* m_Playername;
	Place* m_CityMap;
	
};

#endif