#include "LinkingContext.h"

#include "Utils\CheckedDelete.h"

LinkingContext::LinkingContext() : mNextNetworkId(1)
{
}

LinkingContext::~LinkingContext()
{
	/*for (std::unordered_map<uint32_t, GameObject*>::iterator it = mNetworkIdToGameObjectMap.begin(); it != mNetworkIdToGameObjectMap.end(); ++it)
	{
		delete it->second;
	}*/
	mNetworkIdToGameObjectMap.clear();
	mGameObjectToNetworkIdMap.clear();
}

uint32_t LinkingContext::GetNetworkId(GameObject* inGameObject, bool inShouldCreateIfNotFound)
{
	auto it = mGameObjectToNetworkIdMap.find(inGameObject);
	if (it != mGameObjectToNetworkIdMap.end())
	{
		return it->second;
	}
	else if (inShouldCreateIfNotFound == true)
	{
		uint32_t newNetworkId = GetNextNetworkId();
		AddGameObject(inGameObject, newNetworkId);
		return newNetworkId;
	}
	else
	{
		return 0;
	}
}

void LinkingContext::AddGameObject(GameObject* inGameObject, uint32_t inNetworkId)
{
	mNetworkIdToGameObjectMap[inNetworkId] = inGameObject;
	mGameObjectToNetworkIdMap[inGameObject] = inNetworkId;
}

void LinkingContext::RemoveGameObject(GameObject* inGameObject)
{
	uint32_t networkId = mGameObjectToNetworkIdMap[inGameObject];
	mGameObjectToNetworkIdMap.erase(inGameObject);
	mNetworkIdToGameObjectMap.erase(networkId);
	mUnusedNetworkIds.push_back(networkId);
}

// Network ID reciclyng
uint32_t LinkingContext::GetNextNetworkId()
{
	uint32_t networkId;
	if (mUnusedNetworkIds.empty()) {
		networkId = mNextNetworkId++;
	}
	else {
		networkId = mUnusedNetworkIds.back();
		mUnusedNetworkIds.pop_back();
	}
	return networkId;
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