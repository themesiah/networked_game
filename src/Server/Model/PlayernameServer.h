#pragma once

#ifndef H_PLAYERNAME_SERVER
#define H_PLAYERNAME_SERVER

#include "CommonClasses\PlayerName.h"

class PlayernameServer : public Playername
{
public:
	virtual ~PlayernameServer()
	{

	}
	void SetPlayer(uint32_t aNetworkId, const std::string& aName) {
		m_PlayerId = aNetworkId;
		m_PlayerName = aName;
	}
	CLASS_IDENTIFICATION('PLYN', PlayernameServer);
};

#endif