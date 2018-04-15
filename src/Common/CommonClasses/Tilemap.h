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
			MemberVariable("mTilemapId", EPT_Short, OffsetOf(Tilemap, mTilemapId)),
			MemberVariable("mTiles", EPT_IntArray, OffsetOf(Tilemap, mTiles)),
			MemberVariable("mRenderLayer", EPT_Char, OffsetOf(Tilemap, mRenderLayer))
		}
		);
	}
protected:
	std::vector<int> mTiles;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mTileWidth;
	unsigned int mTileHeight;
	uint16_t mTilemapId;
	unsigned char mRenderLayer;
};

#endif