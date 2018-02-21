#pragma once

#include <inttypes.h>
#include <unordered_map>

class GameObject;
class LinkingContext
{
public:
	LinkingContext();
	~LinkingContext();
	uint32_t GetNetworkId(GameObject* inGameObject);
	GameObject* GetGameObject(uint32_t inNetworkId);
private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap;
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkIdMap;
};