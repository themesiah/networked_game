#pragma once

#ifndef H_REPLICATION_MANAGER
#define H_REPLICATION_MANAGER

#include <vector>
#include <unordered_set>

#include "../Serializer/OutputMemoryBitStream.h"
#include "../Serializer/InputMemoryBitStream.h"

class GameObject;
class LinkingContext;
class CReplicationManager
{
public:
	CReplicationManager();
	virtual ~CReplicationManager();
	void ReplicateWorldState(MemoryStream* inStream, const std::vector<GameObject*>& inAllObjects);
	std::unordered_set<GameObject*> ReceiveReplicatedObjects(MemoryStream* inStream);
private:
	void ReplicateIntoStream(MemoryStream* inStream, GameObject* inGameObject);
	GameObject* ReceiveReplicatedObject(MemoryStream* inStream);
	LinkingContext* mLinkingContext;
	std::unordered_set<GameObject*> mObjectsReplicatedToMe;

	void ReplicateCreate(MemoryStream* inStream, GameObject* inGameObject);
	void ReplicateUpdate(MemoryStream* inStream, GameObject* inGameObject);
	void ReplicateDestroy(MemoryStream*inStream, GameObject* inGameObject);
	void ProcessReplicationAction(MemoryStream* inStream);
};

#endif