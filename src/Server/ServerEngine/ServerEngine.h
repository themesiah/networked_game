#pragma once

#ifndef H_SERVER_ENGINE
#define H_SERVER_ENGINE

#include <vector>
#include <chrono>

#include "Utils\Singleton.h"
#include "Common\SocketUtil.h"
#include "Replication\GameObject.h"

#include "Serializer\OutputMemoryBitStream.h"
#include "Serializer\InputMemoryBitStream.h"
#include "Serializer\PacketStream.h"
#include "../ServerLogger.h"

#define TARGET_FPS 60
#define SEND_INTERVAL 0.1f // How many times the server will send data to the players per second
#define SEGMENT_SIZE 1500

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
const C##Manager& Get##Manager() const { return *m_##Manager; } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Has##Manager() { return m_##Manager != nullptr; } \

/*class OutputMemoryBitStream;
class InputMemoryBitStream;*/
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

class CReplicationManager;
class CMovement;
class CServerEngine : public base::utils::CSingleton<CServerEngine>
{
public:
	virtual ~CServerEngine();
	void Init();
	void Update();

	BUILD_GET_SET_ENGINE_MANAGER(ReplicationManager);
protected:
	CServerEngine();
	friend class base::utils::CSingleton<CServerEngine>;
private:
	std::chrono::monotonic_clock m_Clock;
	std::chrono::monotonic_clock::time_point m_PrevTime;
	TCPSocketPtr m_ListenSocket;
	InputMemoryBitStream* m_InputMs;
	OutputMemoryBitStream* m_OutputMs;
	std::vector<TCPSocketPtr> m_Sockets;
	std::vector<TCPSocketPtr> m_ReadSockets;
	std::vector<TCPSocketPtr> m_WriteSockets;
	std::vector<TCPSocketPtr> m_ErrorSockets;

	void InitSockets();
	void InitReflection();
	void InitDataPos(const TCPSocketPtr& socket);
	void ProcessDataFromClientPos(CPosition* pos, float dt);
	void UpdateSendingSockets(float aDeltaTime);
	void UpdateReceivingSockets(float aDeltaTime);
	void UpdatePackets(float aDeltaTime);
	void ManageDisconnection(TCPSocketPtr socket);
	float m_SendTimer;
	std::map<TCPSocketPtr, CPosition*> m_Positions;
	std::vector<GameObject*> m_GameObjects;
	CMovement* m_Movement;
	std::map<TCPSocketPtr, PacketStream*> m_PacketStreams;
};

#endif