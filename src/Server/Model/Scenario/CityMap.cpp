#include "CityMap.h"

#include <rapidjson\document.h>

#include "../../Network/ClientProxy.h"
#include "../Place.h"
#include "TilemapServer.h"

#include "Utils\FileUtils.h"
#include "../../ServerLogger.h"

#include "../../ServerEngine/ServerEngine.h"

#include "Replication\ReplicationManager.h"
#include "Replication\GameObject.h"



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
	m_Width = document["width"].GetInt();
	m_Height = document["height"].GetInt();
	m_TileWidth = document["tilewidth"].GetInt();
	m_TileHeight = document["tileheight"].GetInt();
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
				tilemapServer->SetData(lTileset, m_TileWidth, m_TileHeight, lTilesVector, m_Width, m_Height, lRenderLayer);
				m_MapObjects.push_back(tilemapServer);
				m_Tilemaps.Add(lLayerName, tilemapServer);
			}
			else {
				m_Tilemaps.Get(lLayerName)->SetData(lTileset, m_TileWidth, m_TileHeight, lTilesVector, m_Width, m_Height, lRenderLayer);
			}
		}
		else {
			const Value& lTilesArray = layers[i]["data"];
			std::vector<bool> lTilesVector;
			for (auto& v : lTilesArray.GetArray())
			{
				if (v.GetInt() == 0) {
					lTilesVector.push_back(false);
				}
				else {
					lTilesVector.push_back(true);
				}
			}
			m_CollisionMap.SetTiles(lTilesVector);
		}
	}
	LOGGER.Log("Done loading %d layers", layersSize);
	return true;
}

void CityMap::Destroy()
{
	for (int i = 0; i < m_Tilemaps.GetCount(); ++i)
	{
		m_Tilemaps.GetIndex(i)->DestroySignal();
	}
	m_Tilemaps.Clear();
	ManageObjectsDestroy();
}

bool CityMap::Reload()
{
	LOGGER.Log("Reloading map with path %s", m_ResourcePath.c_str());
	return Load(m_ResourcePath);
}

void CityMap::ProcessPlace()
{
	Place::ProcessPlace();
	m_Output.Reset();
	m_DeltasOutput.Reset();
	CReplicationManager& lReplicationManager = CServerEngine::GetInstance().GetReplicationManager();
	lReplicationManager.ReplicateWorldDeltas(m_DeltasOutput, m_MapObjects);
	m_DeltasOutput.Close();
	lReplicationManager.ReplicateWorldState(m_Output, m_MapObjects);
	m_Output.Close();
}

void CityMap::OnClientRegister(CClientProxy* aClient)
{
	m_MapObjects.push_back((GameObject*)aClient->GetPlayerController());
	m_MapObjects.push_back((GameObject*)aClient->GetPlayernameServer());
}

void CityMap::OnClientUnregister(CClientProxy* aClient)
{
	GameObject* lPlayerController = (GameObject*)aClient->GetPlayerController();
	GameObject* lPlayernameServer = (GameObject*)aClient->GetPlayernameServer();
	lPlayerController->DestroySignal();
	lPlayernameServer->DestroySignal();
}

void CityMap::Update(const float& dt)
{
	for (const auto& go : m_MapObjects)
	{
		go->Update(dt);
	}
	ProcessPlace();
}