#include "CollisionMap.h"

CollisionMap::CollisionMap()
{

}

CollisionMap::~CollisionMap()
{

}

void CollisionMap::SetTiles(std::vector<bool> aTiles)
{
	mTiles = aTiles;
}

bool CollisionMap::IsTrespassable(const int& index)
{
	return mTiles[index];
}