#pragma once

#ifndef H_CITY_MAP
#define H_CITY_MAP

#include <vector>

#include "Utils\TemplatedMapVector.h"
#include "CollisionMap.h"
#include "../Place.h"

#include "Serializer\InputMemoryBitStream.h"
#include "Serializer\OutputMemoryBitStream.h"

class TilemapServer;
class CityMap : public Place
{
public:
	CityMap();
	virtual ~CityMap();
	virtual bool Reload() override;
	virtual bool Load(const std::string& path) override;
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
	virtual bool IsCity() override
	{
		return true;
	}
	virtual void Update(const float& dt) override;
	virtual void ProcessPlace() override;
	virtual void OnClientRegister(CClientProxy* aClient) override;
	virtual void OnClientUnregister(CClientProxy* aClient) override;
	virtual OutputMemoryBitStream& GetOutput() override
	{
		return m_Output;
	}
	virtual OutputMemoryBitStream& GetDeltasOutput() override
	{
		return m_DeltasOutput;
	};
private:
	std::string m_ResourcePath;
	base::utils::CTemplatedMapVector<TilemapServer> m_Tilemaps;
	CollisionMap m_CollisionMap;
	int m_TileWidth;
	int m_TileHeight;
	int m_Width;
	int m_Height;
	OutputMemoryBitStream m_Output;
	OutputMemoryBitStream m_DeltasOutput;
};

#endif