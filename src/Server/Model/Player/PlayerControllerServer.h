#pragma once

#ifndef H_PLAYER_CONTROLLER_SERVER
#define H_PLAYER_CONTROLLER_SERVER

#include "Replication\GameObject.h"

#include "CommonClasses\PlayerController.h"

#define PLAYER_SPEED 150.f

class CPlayerControllerServer : public CPlayerController
{
public:
	void SetPosition(const float& x, const float& y)
	{
		m_PosX = x;
		m_PosY = y;
		SetDirty();
	}
	void Move(float x, float y, const float& dt)
	{
		if (x != 0.f || y != 0.f)
		{
			m_PosX += x * PLAYER_SPEED * dt;
			m_PosY += y * PLAYER_SPEED * dt;
			SetDirty();
		}
	}
	void SetAnimationId(const uint16_t& aAnimationId)
	{
		m_AnimationId = aAnimationId;
	}
	CLASS_IDENTIFICATION('CPCT', CPlayerControllerServer);
};

#endif