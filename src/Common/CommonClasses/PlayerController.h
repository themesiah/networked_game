#pragma once

#ifndef H_PLAYER_CONTROLLER
#define H_PLAYER_CONTROLLER

#include "Replication\GameObject.h"

class CPlayerController : public GameObject
{
public:
	CPlayerController() : GameObject()
		, m_PosX(100.f)
		, m_PosY(100.f){}
	~CPlayerController() {}
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("m_AnimationId", EPT_Short, OffsetOf(CPlayerController, m_AnimationId)),
			MemberVariable("m_PosX", OffsetOf(CPlayerController, m_PosX), -1000.0f, 0.1f),
			MemberVariable("m_PosY", OffsetOf(CPlayerController, m_PosY), -1000.0f, 0.1f)
		}
		);
	}
protected:
	uint16_t m_AnimationId;
	float m_PosX;
	float m_PosY;
};

#endif