#pragma once

#ifndef H_TILEMAP_SERVER
#define H_TILEMAP_SERVER

#include "CommonClasses\Tilemap.h"

class TilemapServer : public Tilemap
{
public:
	CLASS_IDENTIFICATION('TLMP', TilemapServer);
	void SetData(const uint16_t& tileset, unsigned int tileWidth, unsigned int tileHeight, std::vector<int> tiles, unsigned int width, unsigned int height, unsigned char renderLayer);
};

#endif