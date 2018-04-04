#pragma once

#ifndef H_TILEMAP_SERVER
#define H_TILEMAP_SERVER

#include "CommonClasses\Tilemap.h"

class TilemapServer : public Tilemap
{
public:
	CLASS_IDENTIFICATION('TLMP', TilemapServer);
	void SetData(const std::string& tileset, unsigned int tileWidth, unsigned int tileHeight, const int* tiles, unsigned int width, unsigned int height);
};

#endif