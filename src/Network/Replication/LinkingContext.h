#pragma once

#include <inttypes.h>
#include <unordered_map>

class GameObject;
class LinkingContext
{
public:
	LinkingContext();
	~LinkingContext();
	uint32_t GetNetworkId(GameObject* inGameObject, bool inShouldCreateIfNotFound);
	GameObject* GetGameObject(uint32_t inNetworkId);
	void AddGameObject(GameObject* inGameObject, uint32_t inNetworkId);
	void RemoveGameObject(GameObject* inGameObject);
	uint32_t GetNextNetworkId();
private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkIdMap;
	uint32_t mNextNetworkId;
	std::vector<uint32_t> mUnusedNetworkIds;
};