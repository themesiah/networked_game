#include "../ServerEngine/ServerEngine.h"
#include "../Network/RPCManagerServer.h"
#include "../Network/ClientProxy.h"
#include "../ServerLogger.h"

#include "CommonClasses\Movement.h"
#include "../Model/Player/PlayerControllerServer.h"


void Move(InputMemoryBitStream& inStream, CClientProxy* aClientProxy, float dt)
{
	CPlayerControllerServer::Direction dir;
	inStream.Serialize(dir, 2);
	aClientProxy->GetPlayerController()->MoveTile(dir);
}

template <>
void BindRPCFunctionsServer<CClientProxy>(CRPCManagerServer* inRPCManager)
{
	inRPCManager->RegisterUnwrapPlayerFunction('MOVE', Move);
}