#pragma once

#include "Replication\GameObject.h"

#include "Common\SocketUtil.h"
#include "Common\TCPSocket.h"

class CAnimatedSprite;
class CAnimationSet;

class OutputMemoryBitStream;
class InputMemoryBitStream;
class CPlayerController : GameObject
{
public:
	enum PlayerAnimations {
		MOVE_UP = 0,
		MOVE_DOWN = 1,
		MOVE_RIGHT = 2,
		MOVE_LEFT = 3
	};
	CPlayerController();
	virtual ~CPlayerController();
	void Init();
	void Update(float aDeltaTime);
	void SocketUpdate(float aDeltaTime);
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("m_PosX", OffsetOf(CPlayerController, m_PosX), -1000.0f, 0.1f),
			MemberVariable("m_PosY", OffsetOf(CPlayerController, m_PosY), -1000.0f, 0.1f)
		}
		);
	}
	CLASS_IDENTIFICATION('CPCT', CPlayerController);
private:
	CAnimatedSprite* m_pAnimatedSprite;
	CAnimationSet* m_pAnimationSet;
	float m_PosX;
	float m_PosY;
	float m_Speed;
	size_t m_CurrentAnimation;
	TCPSocketPtr m_Socket;
	std::vector<TCPSocketPtr> m_ReadBlockSockets;
	OutputMemoryBitStream *m_OutputMs;
	InputMemoryBitStream *m_InputMs;
};
