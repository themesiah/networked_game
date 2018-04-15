#pragma once

#ifndef H_CITY_MAP
#define H_CITY_MAP

#include "Utils\TemplatedMapVector.h"

class TilemapServer;
class CollisionMap;
class CityMap// : public LoadableResource
{
public:
	CityMap();
	virtual ~CityMap();
	bool Reload();
	bool Load(const std::string& path);
	void Destroy();
private:
	std::string m_ResourcePath;
	base::utils::CTemplatedMapVector<TilemapServer> m_Tilemaps;
	//CollisionMap* m_CollisionMap;
};

#endif