#include "../Engine/Engine.h"
#include "Common\RPCManager.h"
#include "../Engine/ChatManager.h"

void Chat(InputMemoryBitStream& inStream)
{
	int lMessageQuantity;
	inStream.Serialize(lMessageQuantity);
	CChatManager& lChatManager = CEngine::GetInstance().GetChatManager();
	for (int i = 0; i < lMessageQuantity; ++i)
	{
		std::string lName, lMessage;
		inStream.Serialize(lName);
		inStream.Serialize(lMessage);
		lChatManager.AddMessage("%s: %s", lName.c_str(), lMessage.c_str());
	}
}

template <>
void BindRPCFunctions<CChatManager>(CRPCManager* inRPCManager)
{
	inRPCManager->RegisterUnwrapFunction('CHAT', Chat);
}