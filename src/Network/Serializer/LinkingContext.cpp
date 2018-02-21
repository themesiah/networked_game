#include "LinkingContext.h"

LinkingContext::LinkingContext()
{
	mNetworkIdToGameObjectMap = std::unordered_map<uint32_t, GameObject*>();
	mGameObjectToNetworkIdMap = std::unordered_map<GameObject*, uint32_t>();
}

LinkingContext::~LinkingContext()
{
	mNetworkIdToGameObjectMap.clear();
	mGameObjectToNetworkIdMap.clear();
}

uint32_t LinkingContext::GetNetworkId(GameObject* inGameObject)
{
	auto it = mGameObjectToNetworkIdMap.find(inGameObject);
	if (it != mGameObjectToNetworkIdMap.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}

GameObject* LinkingContext::GetGameObject(uint32_t inNetworkId)
{
	auto it = mNetworkIdToGameObjectMap.find(inNetworkId);
	if (it != mNetworkIdToGameObjectMap.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}