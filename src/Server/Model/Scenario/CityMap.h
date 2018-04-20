#pragma once

#ifndef H_CITY_MAP
#define H_CITY_MAP

#include "Utils\TemplatedMapVector.h"
#include "CollisionMap.h"

class TilemapServer;
class CityMap
{
public:
	CityMap();
	virtual ~CityMap();
	bool Reload();
	bool Load(const std::string& path);
	void Destroy();
	int GetMapWidth()
	{
		return m_Width;
	}
	int GetMapHeight()
	{
		return m_Height;
	}
	int GetTileWidth()
	{
		return m_TileWidth;
	}
	int GetTileHeight()
	{
		return m_TileHeight;
	}
	bool IsTrespassable(const int& index)
	{
		return m_CollisionMap.IsTrespassable(index);
	}
private:
	std::string m_ResourcePath;
	base::utils::CTemplatedMapVector<TilemapServer> m_Tilemaps;
	CollisionMap m_CollisionMap;
	int m_TileWidth;
	int m_TileHeight;
	int m_Width;
	int m_Height;
};

#endif