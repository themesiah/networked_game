#pragma once

#ifndef H_TILEMAP
#define H_TILEMAP

#include "Replication\GameObject.h"

class Tilemap : public GameObject
{
public:
	virtual ~Tilemap()
	{
		mTiles.clear();
	}
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("mWidth", EPT_Int, OffsetOf(Tilemap, mWidth)),
			MemberVariable("mHeight", EPT_Int, OffsetOf(Tilemap, mHeight)),
			MemberVariable("mTileWidth", EPT_Int, OffsetOf(Tilemap, mTileWidth)),
			MemberVariable("mTileHeight", EPT_Int, OffsetOf(Tilemap, mTileHeight)),
			MemberVariable("mTilesetName", EPT_String, OffsetOf(Tilemap, mTilesetName)),
			MemberVariable("mTiles", EPT_IntArray, OffsetOf(Tilemap, mTiles))
		}
		);
	}
protected:
	std::vector<int> mTiles;
	unsigned int mWidth;
	unsigned int mHeight;
	std::string mTilesetName;
	unsigned int mTileWidth;
	unsigned int mTileHeight;
};

#endif