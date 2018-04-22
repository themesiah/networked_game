#include "../ServerEngine/ServerEngine.h"
#include "../Network/RPCManagerServer.h"
#include "../Network/ClientProxy.h"
#include "../Model/Place.h"
#include "../ServerLogger.h"

void Chat(InputMemoryBitStream& inStream, CClientProxy* aClientProxy, float dt)
{
	aClientProxy->GetPlace()->ManageChatRPC(inStream, aClientProxy);
}

template <>
void BindRPCFunctionsServer<Place>(CRPCManagerServer* inRPCManager)
{
	inRPCManager->RegisterUnwrapPlayerFunction('CHAT', Chat);
}