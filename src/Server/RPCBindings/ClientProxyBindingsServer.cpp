#include "../ServerEngine/ServerEngine.h"
#include "../Network/RPCManagerServer.h"
#include "../Network/ClientProxy.h"
#include "../ServerLogger.h"

#include "CommonClasses\Movement.h"
#include "../Model/Player/PlayerControllerServer.h"


void Move(InputMemoryBitStream& inStream, CClientProxy* aClientProxy, float dt)
{
	CMovement lMovement;
	lMovement.SerializeRead(inStream);
	auto mov = lMovement.GetMovement();
	aClientProxy->GetPlayerController()->Move(mov[0], mov[1], dt);
}

template <>
void BindRPCFunctionsServer<CClientProxy>(CRPCManagerServer* inRPCManager)
{
	inRPCManager->RegisterUnwrapPlayerFunction('MOVE', Move);
}