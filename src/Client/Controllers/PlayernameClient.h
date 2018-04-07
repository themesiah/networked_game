#pragma once

#ifndef H_PLAYERNAME_CLIENT
#define H_PLAYERNAME_CLIENT

#include "CommonClasses\PlayerName.h"
#include "PlayerControllerClient.h"

class CPlayerController;
class LinkingContext;
class PlayernameClient : public Playername
{
public:
	PlayernameClient();
	virtual ~PlayernameClient();
	void Update(float aDeltaTime) override;
	CLASS_IDENTIFICATION('PLYN', PlayernameClient);
private:
	LinkingContext* m_LinkingContext;
	bool m_Set;
};

#endif