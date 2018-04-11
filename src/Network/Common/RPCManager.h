#pragma once

#ifndef H_RPC_MANAGER
#define H_RPC_MANAGER

#include <unordered_map>

#include "../Serializer/InputMemoryBitStream.h"
#include "Debug\CustomAssert.h"

typedef void(*RPCUnwrapFunc)(InputMemoryBitStream&);

class CRPCManager
{
public:
	void RegisterUnwrapFunction(uint32_t inName, RPCUnwrapFunc inFunc)
	{
		Assert(mNameToRPCTable.find(inName) == mNameToRPCTable.end(), "RPC function already exist.");
		mNameToRPCTable[inName] = inFunc;
	}
	void ProcessRPC(InputMemoryBitStream& inStream)
	{
		uint32_t name;
		inStream.Serialize(name);
		mNameToRPCTable[name](inStream);
	}
private:
	std::unordered_map<uint32_t, RPCUnwrapFunc> mNameToRPCTable;
};
template < typename T > void BindRPCFunctions(CRPCManager *aRPC);

#endif