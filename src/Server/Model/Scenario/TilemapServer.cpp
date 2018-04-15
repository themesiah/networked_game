#include "TilemapServer.h"

void TilemapServer::SetData(const uint16_t& tileset, unsigned int tileWidth, unsigned int tileHeight, std::vector<int> tiles, unsigned int width, unsigned int height, unsigned char renderLayer)
{
	mTilemapId = tileset;
	mTileWidth = tileWidth;
	mTileHeight = tileHeight;
	mWidth = width;
	mHeight = height;
	mTiles = tiles;
	mRenderLayer = renderLayer;
	SetDirty();
}