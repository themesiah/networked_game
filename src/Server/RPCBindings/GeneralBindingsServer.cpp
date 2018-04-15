#include "../ServerEngine/ServerEngine.h"
#include "../Network/RPCManagerServer.h"
#include "../Network/ClientProxy.h"
#include "../ServerLogger.h"

#include "../Model/Scenario/CityMap.h"

const std::string SECRET = "secret";
void ServerShutdown(InputMemoryBitStream& inStream, CClientProxy* aClientProxy, float dt)
{
	std::string secret;
	inStream.Serialize(secret);
	if (secret == SECRET) {
		LOGGER.Log("Called ServerShutdown. Shutting down.");
		CServerEngine::GetInstance().Shutdown();
	}
	else {
		LOGGER.Warning("A player tried to call a system RPC (%s) with an incorrect secret.", "ServerShutdown");
	}
}

void MapReload(InputMemoryBitStream& inStream, CClientProxy* aClientProxy, float dt)
{
	std::string secret;
	inStream.Serialize(secret);
	if (secret == SECRET) {
		LOGGER.Log("Called MapReload. Reloading map.");
		CServerEngine::GetInstance().GetCityMap()->Reload();
	}
	else {
		LOGGER.Warning("A player tried to call a system RPC (%s) with an incorrect secret.", "MapReload");
	}
}

template <>
void BindRPCFunctionsServer<CServerEngine>(CRPCManagerServer* inRPCManager)
{
	inRPCManager->RegisterUnwrapPlayerFunction('SDWN', ServerShutdown);
	inRPCManager->RegisterUnwrapPlayerFunction('MRLD', MapReload);
}