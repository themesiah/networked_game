#pragma once

#ifndef H_SERVER_ENGINE
#define H_SERVER_ENGINE

#include <vector>
#include <chrono>

#include "Utils\Singleton.h"
#include "Common\SocketUtil.h"
#include "Serializer\SerializableObject.h"

#define TARGET_FPS 10
#define SEGMENT_SIZE 1024

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
const C##Manager& Get##Manager() const { return *m_##Manager; } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Has##Manager() { return m_##Manager != nullptr; } \

class CPosition : public SerializableObject {
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
	enum { kClassId = 'CPOS' };
	uint32_t GetClassId() override {
		return kClassId;
	}
};

class CServerEngine : public base::utils::CSingleton<CServerEngine>
{
public:
	virtual ~CServerEngine();
	void Init();
	void Update();
protected:
	CServerEngine();
	friend class base::utils::CSingleton<CServerEngine>;
private:
	std::chrono::monotonic_clock m_Clock;
	std::chrono::monotonic_clock::time_point m_PrevTime;
	TCPSocketPtr m_ListenSocket;

	std::vector<TCPSocketPtr> m_ReadBlockSockets;
	std::vector<TCPSocketPtr> m_ReadableSockets;

	std::vector<TCPSocketPtr> m_WriteBlockSockets;
	std::vector<TCPSocketPtr> m_WritableSockets;

	std::vector<TCPSocketPtr> m_ErrorBlockSockets;
	std::vector<TCPSocketPtr> m_ErrorableSockets;

	InputMemoryBitStream* m_InputMs;
	OutputMemoryBitStream* m_OutputMs;

	void InitSockets();
	void InitReflection();
	void InitDataPos(const TCPSocketPtr& socket);
	void ProcessDataFromClientPos(const TCPSocketPtr& socket, char* segment, int dataReceived, CPosition* pos);
	std::vector<CPosition*> m_Positions;
};

#endif