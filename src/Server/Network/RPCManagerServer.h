#pragma once

#ifndef H_RPC_MANAGER_SERVER
#define H_RPC_MANAGER_SERVER

#include "Common\RPCManager.h"

class CClientProxy;
typedef void(*RPCUnwrapPlayerFunc)(InputMemoryBitStream&, CClientProxy*);

class CRPCManagerServer : public CRPCManager
{
public:
	void RegisterUnwrapPlayerFunction(uint32_t inName, RPCUnwrapPlayerFunc inFunc)
	{
		Assert(mNameToPlayerRPCTable.find(inName) == mNameToPlayerRPCTable.end(), "Player RPC function already exist.");
		mNameToPlayerRPCTable[inName] = inFunc;
	}
	void ProcessPlayerRPC(InputMemoryBitStream& inStream, CClientProxy* inClient)
	{
		uint32_t name;
		inStream.Serialize(name);
		Assert(mNameToPlayerRPCTable.find(name) != mNameToPlayerRPCTable.end(), "Player RPC function does not exist.");
		if (mNameToPlayerRPCTable.find(name) != mNameToPlayerRPCTable.end()) {
			mNameToPlayerRPCTable[name](inStream, inClient);
		}
	}
private:
	std::unordered_map<uint32_t, RPCUnwrapPlayerFunc> mNameToPlayerRPCTable;
};
template < typename T > void BindRPCFunctionsServer(CRPCManagerServer *aRPC);

#endif