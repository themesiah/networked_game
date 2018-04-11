#include "../Engine/Engine.h"
#include "Common\RPCManager.h"

/*void ServerShutdown(InputMemoryBitStream& inStream)
{
	std::string secret;
	inStream.Serialize(secret);
	if (secret == "secret") {
		CServerEngine::GetInstance().Shutdown();
	}
}*/

template <>
void BindRPCFunctions<CEngine>(CRPCManager* inRPCManager)
{
	//inRPCManager->RegisterUnwrapFunction('SDWN', ServerShutdown);
}