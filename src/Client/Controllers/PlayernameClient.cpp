#include "PlayernameClient.h"

#include "../Engine/Engine.h"
#include "Replication\ReplicationManager.h"
#include "Replication\LinkingContext.h"

PlayernameClient::PlayernameClient() :
m_Set(false)
{
	m_LinkingContext = CEngine::GetInstance().GetReplicationManager().GetLinkingContext();
}

PlayernameClient::~PlayernameClient()
{

}

void PlayernameClient::Update(float aDeltaTime)
{
	if (m_PlayerId != 0 && !m_Set)
	{
		GameObject* go = m_LinkingContext->GetGameObject(m_PlayerId);
		if (go != nullptr)
		{
			CPlayerControllerClient* lPlayer = static_cast<CPlayerControllerClient*>(go);
			if (lPlayer != nullptr)
			{
				lPlayer->SetName(m_PlayerName);
				m_Set = true;
			}
		}
	}
}