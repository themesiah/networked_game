#pragma once

#ifndef H_ANIMATION_SET
#define H_ANIMATION_SET

#include <map>
#include <iostream>

#include "../Engine/Resource.h"

class CAnimation;
class CAnimationSet : public IResource
{
public:
	CAnimationSet();
	virtual ~CAnimationSet();
	void AddAnimation(size_t aIndex, CAnimation* aAnimation);
	CAnimation* GetAnimation(size_t aIndex) const;

	virtual bool LoadResource(const std::string& aResourceName) override;
private:
	std::map<size_t, CAnimation*> m_AnimationsMap;
};

#endif