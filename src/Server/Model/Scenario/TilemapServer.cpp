#include "TilemapServer.h"

void TilemapServer::SetData(const uint16_t& tileset, unsigned int tileWidth, unsigned int tileHeight, const int* tiles, unsigned int width, unsigned int height)
{
	mTilemapId = tileset;
	mTileWidth = tileWidth;
	mTileHeight = tileHeight;
	mWidth = width;
	mHeight = height;
	mTiles = std::vector<int>(tiles, tiles + (width*height));
	Undirty(); // Never dirty. Only received with the main game state.
}