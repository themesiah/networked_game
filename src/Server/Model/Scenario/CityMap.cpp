#include "CityMap.h"

#include <rapidjson\document.h>

#include "Utils\FileUtils.h"

#include "../../ServerEngine/ServerEngine.h"
#include "TilemapServer.h"
#include "../../ServerLogger.h"

using namespace rapidjson;

CityMap::CityMap()
{

}

CityMap::~CityMap()
{
	Destroy();
}

bool CityMap::Load(const std::string& aPath)
{
	LOGGER.Log("Loading map with path %s", aPath.c_str());
	m_ResourcePath = aPath;
	const std::string COLLISION_LAYER_NAME = "CollisionLayer";
	std::string jsonData = base::utils::GetTextFromFile(aPath);
	Document document;
	document.Parse(jsonData.c_str(), jsonData.size());
	int lWidth, lHeight, lTilewidth, lTileheight;
	lWidth = document["width"].GetInt();
	lHeight = document["height"].GetInt();
	lTilewidth = document["tilewidth"].GetInt();
	lTileheight = document["tileheight"].GetInt();
	const Value& layers = document["layers"];
	SizeType layersSize = layers.Size();
	for (SizeType i = 0; i < layersSize; ++i)
	{
		std::string lLayerName = layers[i]["name"].GetString();
		if (lLayerName != COLLISION_LAYER_NAME)
		{
			uint16_t lTileset = layers[i]["tileset"].GetUint();
			unsigned char lRenderLayer = layers[i]["zindex"].GetUint();
			const Value& lTilesArray = layers[i]["data"];
			std::vector<int> lTilesVector;
			for (auto& v : lTilesArray.GetArray())
			{
				lTilesVector.push_back(v.GetInt());
			}
			if (!m_Tilemaps.Exist(lLayerName)) {
				TilemapServer* tilemapServer = new TilemapServer();
				tilemapServer->SetData(lTileset, lTilewidth, lTileheight, lTilesVector, lWidth, lHeight, lRenderLayer);
				CServerEngine::GetInstance().GetGameObjects()->push_back(tilemapServer);
				m_Tilemaps.Add(lLayerName, tilemapServer);
			}
			else {
				m_Tilemaps.Get(lLayerName)->SetData(lTileset, lTilewidth, lTileheight, lTilesVector, lWidth, lHeight, lRenderLayer);
			}
		}
		// TODO: Collision layer!
	}
	LOGGER.Log("Done loading %d layers", layersSize);
	return true;
}

void CityMap::Destroy()
{
	//delete m_CollisionMap;
	for (int i = 0; i < m_Tilemaps.GetCount(); ++i)
	{
		m_Tilemaps.GetIndex(i)->DestroySignal();
	}
	m_Tilemaps.Clear();
}

bool CityMap::Reload()
{
	LOGGER.Log("Reloading map with path %s", m_ResourcePath.c_str());
	return Load(m_ResourcePath);
}