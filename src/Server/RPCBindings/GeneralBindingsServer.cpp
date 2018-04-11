#include "../ServerEngine/ServerEngine.h"
#include "../Network/RPCManagerServer.h"
#include "../Network/ClientProxy.h"
#include "../ServerLogger.h"


void ServerShutdown(InputMemoryBitStream& inStream, CClientProxy* aClientProxy)
{
	const std::string SECRET = "secret";
	std::string secret = "";
	inStream.Serialize(secret);
	if (secret == SECRET) {
		LOGGER.Log("Called ServerShutdown. Shutting down.");
		CServerEngine::GetInstance().Shutdown();
	}
	else {
		LOGGER.Warning("A player tried to call a system RPC with an incorrect secret.");
	}
}

template <>
void BindRPCFunctionsServer<CServerEngine>(CRPCManagerServer* inRPCManager)
{
	inRPCManager->RegisterUnwrapPlayerFunction('SDWN', ServerShutdown);
}