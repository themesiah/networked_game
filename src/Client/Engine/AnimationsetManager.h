#pragma once

#ifndef H_ANIMATIONSET_MANAGER
#define H_ANIMATIONSET_MANAGER

#include "ResourcesManager.h"
#include "Engine.h"
#include "../Graphics/AnimationSet.h"

class CAnimationsetManager : public CResourcesManager<CAnimationSet>
{
public:
	CAnimationSet* LoadAnimation(const std::string& aPath)
	{
		if (m_Resources.Exist(aPath)) {
			return Get(aPath);
		}
		else {
			CAnimationSet* lNewAnimation = new CAnimationSet();
			if (lNewAnimation->LoadResource(aPath)) {
				Update(aPath, lNewAnimation);
				return lNewAnimation;
			}
			else {
				return nullptr;
			}
		}
	}

	CAnimationSet* LoadAnimation(const uint16_t& aResourceId)
	{
		return LoadAnimation(CEngine::GetInstance().GetResourceTable(CResourcesTable::ANIMATIONS)->GetResourcePath(aResourceId));
	}
};

#endif