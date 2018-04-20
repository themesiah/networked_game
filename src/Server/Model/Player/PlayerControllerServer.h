#pragma once

#ifndef H_PLAYER_CONTROLLER_SERVER
#define H_PLAYER_CONTROLLER_SERVER

#include "Replication\GameObject.h"

#include "CommonClasses\PlayerController.h"

#define PLAYER_SPEED 150.f

class CityMap;
class CPlayerControllerServer : public CPlayerController
{
public:
	enum Direction : uint8_t
	{
		LEFT = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3
	};
	struct CoordinatePair
	{
		int x;
		int y;
	};
	struct PositionPair
	{
		float x;
		float y;
	};
	void Move(float x, float y, const float& dt);
	void SetAnimationId(const uint16_t& aAnimationId);
	CLASS_IDENTIFICATION('CPCT', CPlayerControllerServer);
	void Init(CityMap* aCityMap);
	void MoveTile(Direction aDirection);
	int MapToIndex(int x, int y);
	CoordinatePair IndexToMap(int index);
	PositionPair MapToPosition(int x, int y);
	void Update(const float& dt);
private:
	unsigned int m_CurrentTile;
	unsigned int m_NextTile;
	CityMap* m_CityMap;
};

#endif