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
	void ReplicateWorldState(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects);
	void ReplicateWorldDeltas(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects);
	std::unordered_set<GameObject*> ReceiveReplicatedObjects(InputMemoryBitStream& inStream);
	std::unordered_set<GameObject*> ReceiveReplicatedDeltas(InputMemoryBitStream& inStream);
	LinkingContext* GetLinkingContext();
private:
	void ReplicateIntoStream(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& inStream);
	LinkingContext* mLinkingContext;
	std::unordered_set<GameObject*> mObjectsReplicatedToMe;

	void ReplicateCreate(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ReplicateUpdate(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ReplicateDestroy(OutputMemoryBitStream& inStream, GameObject* inGameObject);
	void ProcessReplicationAction(InputMemoryBitStream& inStream);
};

#endif