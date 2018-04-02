#pragma once

#ifndef H_REPLICATION_HEADER
#define H_REPLICATION_HEADER

#include <inttypes.h>

#include "../Serializer/OutputMemoryBitStream.h"
#include "../Serializer/InputMemoryBitStream.h"

enum ReplicationAction : uint8_t
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
	void Write(OutputMemoryBitStream& inStream);
	void Read(InputMemoryBitStream& inStream);
};

// MemoryStream is output
void ReplicationHeader::Write(OutputMemoryBitStream& inStream)
{
	inStream.Serialize(mReplicationAction, 3);
	inStream.Serialize(mNetworkId);
	if (mReplicationAction != RA_Destroy)
	{
		inStream.Serialize(mClassId);
	}
}

// MemoryStream is input
void ReplicationHeader::Read(InputMemoryBitStream& inStream)
{
	inStream.Serialize(mReplicationAction, 3);
	inStream.Serialize(mNetworkId);
	if (mReplicationAction != RA_Destroy)
	{
		inStream.Serialize(mClassId);
	}
};

#endif