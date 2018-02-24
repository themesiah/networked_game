#pragma once

#include <map>

class CAnimation;
class CAnimationSet {
public:
	CAnimationSet();
	virtual ~CAnimationSet();
	void AddAnimation(size_t aIndex, CAnimation* aAnimation);
	CAnimation* GetAnimation(size_t aIndex) const;
private:
	std::map<size_t, CAnimation*> m_AnimationsMap;
};