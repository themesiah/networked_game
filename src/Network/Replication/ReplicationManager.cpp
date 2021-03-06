#include "ReplicationManager.h"

#include "GameObject.h"
#include "LinkingContext.h"
#include "Packet.h"
#include "ObjectCreationRegistry.h"
#include "ReplicationHeader.h"
#include "../Common/RPCManager.h"

CReplicationManager::CReplicationManager()
{
	mLinkingContext = new LinkingContext();
}

void CReplicationManager::SetRPCManager(CRPCManager* aRPCManager)
{
	m_RPCManager = aRPCManager;
}

CReplicationManager::~CReplicationManager()
{
	delete mLinkingContext;
	mObjectsReplicatedToMe.clear();
}

LinkingContext* CReplicationManager::GetLinkingContext()
{
	return mLinkingContext;
}

// Memory stream is output
void CReplicationManager::ReplicateIntoStream(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	//write game object id
	inStream.Serialize(mLinkingContext->GetNetworkId(inGameObject, true));
	//write game object class
	inStream.Serialize(inGameObject->GetClassId());
	//write game object data
	inGameObject->SerializeWrite(inStream);
}

void CReplicationManager::ReplicateWorldState(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects)
{
	//tag as replication data
	inStream.Serialize(PT_ReplicationData, PACKET_BIT_SIZE);
	//write each object
	for (GameObject* go : inAllObjects)
	{
		ReplicateIntoStream(inStream, go);
	}
}

void CReplicationManager::ReplicateWorldDeltas(OutputMemoryBitStream& inStream, const std::vector<GameObject*>& inAllObjects)
{
	//tag as replication data
	inStream.Serialize(PT_ReplicationDeltas, PACKET_BIT_SIZE);
	//write each object
	for (GameObject* go : inAllObjects)
	{
		GameObject::DirtyType lDirty = go->GetDirty();
		switch (lDirty)
		{
		case GameObject::DirtyType::UPDATE:
			ReplicateUpdate(inStream, go);
			go->Undirty();
			break;
		case GameObject::DirtyType::CREATE:
			ReplicateCreate(inStream, go);
			go->Undirty();
			break;
		case GameObject::DirtyType::DESTROY:
			ReplicateDestroy(inStream, go);
			go->PrepareToDestroy();
			break;
			// PREPARE TO DESTROY and NOT DIRTY cases not handled
		}
	}
}

// Memory stream is output
void CReplicationManager::ReplicateCreate(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Create, mLinkingContext->GetNetworkId(inGameObject, true), inGameObject->GetClassId());
	rh.Write(inStream);
	inGameObject->SerializeWrite(inStream);
}

// Memory stream is output
void CReplicationManager::ReplicateUpdate(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Update, mLinkingContext->GetNetworkId(inGameObject, false), inGameObject->GetClassId());
	rh.Write(inStream);
	inGameObject->SerializeWrite(inStream);
}

// Memory stream is output
void CReplicationManager::ReplicateDestroy(OutputMemoryBitStream& inStream, GameObject* inGameObject)
{
	ReplicationHeader rh(RA_Destroy, mLinkingContext->GetNetworkId(inGameObject, false));
	rh.Write(inStream);
}

// Memory stream is input
std::unordered_set<GameObject*> CReplicationManager::ReceiveReplicatedObjects(InputMemoryBitStream& inStream)
{
	std::unordered_set<GameObject*> receivedObjects;
	while (inStream.GetRemainingDataSize() >= 32) //4*8 bits
	{
		GameObject* receivedGameObject = ReceiveReplicatedObject(inStream);
		receivedObjects.insert(receivedGameObject);
	}
	//now run through mObjectsReplicatedToMe.
	//if an object isn�t in the recently replicated set,
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
GameObject* CReplicationManager::ReceiveReplicatedObject(InputMemoryBitStream& inStream){
	uint32_t networkId;
	uint32_t classId;
	inStream.Serialize(networkId);
	inStream.Serialize(classId);
	GameObject* go = mLinkingContext->GetGameObject(networkId);
	if (!go)
	{
		go = ObjectCreationRegistry::GetInstance().CreateGameObject(classId);
		mLinkingContext->AddGameObject(go, networkId);
	}
	//now read update
	go->SerializeRead(inStream);
	//return gameobject so we can track it was received in packet
	return go;
}

std::unordered_set<GameObject*> CReplicationManager::ReceiveReplicatedDeltas(InputMemoryBitStream& inStream)
{
	mObjectsReplicatedToMe.clear();
	while (inStream.GetRemainingDataSize() >= 34) //4*8 + 2 bits
	{
		ProcessReplicationAction(inStream);
	}
	return mObjectsReplicatedToMe;
}

// Memory stream is input
void CReplicationManager::ProcessReplicationAction(InputMemoryBitStream& inStream)
{
	ReplicationHeader rh;
	rh.Read(inStream);
	switch (rh.mReplicationAction)
	{
	case RA_Create:
	{
		GameObject* go = ObjectCreationRegistry::GetInstance().CreateGameObject(rh.mClassId);
		mLinkingContext->AddGameObject(go, rh.mNetworkId);
		go->SerializeRead(inStream);
		mObjectsReplicatedToMe.insert(go);
		break;
	}
	case RA_Update:
	{
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);
		//we might have not received the create yet,
		//so serialize into a dummy to advance read head
		if (go)
		{
			go->SerializeRead(inStream);
		}
		else
		{
			uint32_t classId = rh.mClassId;
			go = ObjectCreationRegistry::GetInstance().CreateGameObject(classId);
			go->SerializeRead(inStream);
			delete go;
		}
		break;
	}
	case RA_Destroy:
	{
		GameObject* go = mLinkingContext->GetGameObject(rh.mNetworkId);
		mLinkingContext->RemoveGameObject(go);
		//mObjectsReplicatedToMe.erase(go);
		go->Destroy();
		break;
	}
	case RA_RPC:
	{
		m_RPCManager->ProcessRPC(inStream);
		break;
	}
	default:
		//not handled by us
		break;
	}
}