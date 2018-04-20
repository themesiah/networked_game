#pragma once

#ifndef H_COLLISION_MAP
#define H_COLLISION_MAP

#include <vector>

class CollisionMap
{
public:
	CollisionMap();
	virtual ~CollisionMap();
	void SetTiles(std::vector<bool> aTiles);
	bool IsTrespassable(const int& index);
private:
	std::vector<bool> mTiles;
};

#endif