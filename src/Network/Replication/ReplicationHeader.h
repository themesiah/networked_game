#pragma once

#ifndef H_REPLICATION_HEADER
#define H_REPLICATION_HEADER

#include <inttypes.h>

#include "../Serializer/OutputMemoryBitStream.h"
#include "../Serializer/InputMemoryBitStream.h"

enum ReplicationAction
{
	RA_Create,
	RA_Update,
	RA_Destroy,
	RA_MAX
};

class ReplicationHeader
{
public:
	ReplicationHeader() {}
	ReplicationHeader(ReplicationAction inRA, uint32_t inNetworkId, uint32_t inClassId = 0) :
		mReplicationAction(inRA),
		mNetworkId(inNetworkId),
		mClassId(inClassId)
	{}
	ReplicationAction mReplicationAction;
	uint32_t mNetworkId;
	uint32_t mClassId;
	void Write(MemoryStream* inStream);
	void Read(MemoryStream* inStream);
};

// MemoryStream is output
void ReplicationHeader::Write(MemoryStream* inStream)
{
	uint8_t replicationAction = mReplicationAction;
	inStream->Serialize(mReplicationAction, 2);
	inStream->Serialize(mNetworkId);
	if (mReplicationAction != RA_Destroy)
	{
		inStream->Serialize(mClassId);
	}
}

// MemoryStream is input
void ReplicationHeader::Read(MemoryStream* inStream)
{
	inStream->Serialize(mReplicationAction, 2);
	inStream->Serialize(mNetworkId);
	if (mReplicationAction != RA_Destroy)
	{
		inStream->Serialize(mClassId);
	}
};

#endif