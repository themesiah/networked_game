#include "ReplicationManager.h"

#include "GameObject.h"
#include "LinkingContext.h"
#include "Packet.h"
#include "ObjectCreationRegistry.h"
#include "ReplicationHeader.h"

CReplicationManager::CReplicationManager()
{
	mLinkingContext = new LinkingContext();
}

CReplicationManager::~CReplicationManager()
{
	delete mLinkingContext;
	mObjectsReplicatedToMe.clear();
}

// Memory stream is output
void CReplicationManager::ReplicateIntoStream(MemoryStream* inStream, GameObject* inGameObject)
{
	uint32_t nid = mLinkingContext->GetNetworkId(inGameObject, true);
	uint32_t cid = inGameObject->GetClassId();
	//write game object id
	inStream->Serialize(nid);
	//write game object class
	inStream->Serialize(cid);
	//write game object data
	inGameObject->Serialize(inStream);
}

// Memory stream is output
void CReplicationManager::ReplicateWorldState(MemoryStream* inStream, const std::vector<GameObject*>& inAllObjects)
{
	//tag as replication data
	uint8_t packetType = PT_ReplicationData;
	inStream->Serialize(packetType, 2);
	//write each object
	for (GameObject* go : inAllObjects)
	{
		ReplicateIntoStream(inStream, go);
	}
}

// Memory stream is output
void CReplicationManager::ReplicateCreate(MemoryStream* inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Create, mLinkingContext->GetNetworkId(inGameObject, true), inGameObject->GetClassId());
	rh.Write(inStream);
	inGameObject->Serialize(inStream);
}

// Memory stream is output
void CReplicationManager::ReplicateUpdate(MemoryStream* inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Update, mLinkingContext->GetNetworkId(inGameObject, false), inGameObject->GetClassId());
	rh.Write(inStream);
	inGameObject->Serialize(inStream);
}

// Memory stream is output
void CReplicationManager::ReplicateDestroy(MemoryStream* inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Destroy, mLinkingContext->GetNetworkId(inGameObject, false));
	rh.Write(inStream);
}

// Memory stream is input
std::unordered_set<GameObject*> CReplicationManager::ReceiveReplicatedObjects(MemoryStream* inStream)
{
	std::unordered_set<GameObject*> receivedObjects;
	while (((InputMemoryBitStream*)inStream)->GetRemainingDataSize() >= 32)
	{
		GameObject* receivedGameObject = ReceiveReplicatedObject(inStream);
		receivedObjects.insert(receivedGameObject);
	}
	//now run through mObjectsReplicatedToMe.
	//if an object isn’t in the recently replicated set,
	//destroy it
	for (GameObject* go : mObjectsReplicatedToMe)
	{
		if (receivedObjects.find(go) == receivedObjects.end())
		{
			mLinkingContext->RemoveGameObject(go);
			go->Destroy();
			delete go;
		}
	}
	mObjectsReplicatedToMe = receivedObjects;
	return mObjectsReplicatedToMe;
}

// Memory stream is input
GameObject* CReplicationManager::ReceiveReplicatedObject(MemoryStream* inStream){
	uint32_t networkId;
	uint32_t classId;
	inStream->Serialize(networkId);
	inStream->Serialize(classId);
	GameObject* go = mLinkingContext->GetGameObject(networkId);
	if (!go)
	{
		go = ObjectCreationRegistry::GetInstance().CreateGameObject(classId);
		mLinkingContext->AddGameObject(go, networkId);
	}
	//now read update
	go->Serialize(inStream);
	//return gameobject so we can track it was received in packet
	return go;
}

// Memory stream is input
void CReplicationManager::ProcessReplicationAction(MemoryStream* inStream)
{
	ReplicationHeader rh;
	rh.Read(inStream);
	switch (rh.mReplicationAction)
	{
	case RA_Create:
	{
		GameObject* go = ObjectCreationRegistry::GetInstance().CreateGameObject(rh.mClassId);
		mLinkingContext->AddGameObject(go, rh.mNetworkId);
		go->Serialize(inStream);
		break;
	}
	case RA_Update:
	{
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);
		//we might have not received the create yet,
		//so serialize into a dummy to advance read head
		if (go)
		{
			go->Serialize(inStream);
		}
		else
		{
			uint32_t classId = rh.mClassId;
			go = ObjectCreationRegistry::GetInstance().CreateGameObject(classId);
			go->Serialize(inStream);
			delete go;
		}
		break;
	}
	case RA_Destroy:
	{
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);
		mLinkingContext->RemoveGameObject(go);
		go->Destroy();
		break;
	}
	default:
		//not handled by us
		break;
	}
}