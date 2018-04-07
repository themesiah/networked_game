#pragma once

#ifndef H_PLAYERNAME
#define H_PLAYERNAME

#include "Replication\GameObject.h"

class Playername : public GameObject
{
public:
	virtual ~Playername()
	{

	}
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("mPlayerId", EPT_Int, OffsetOf(Playername, m_PlayerId)),
			MemberVariable("mPlayerName", EPT_String, OffsetOf(Playername, m_PlayerName))
		});
	}
protected:
	uint32_t m_PlayerId;
	std::string m_PlayerName;
};

#endif