#include "AnimationSet.h"

CAnimationSet::CAnimationSet()
{

}

CAnimationSet::~CAnimationSet()
{
	for (std::map< size_t, CAnimation* >::iterator itb = m_AnimationsMap.begin(), ite = m_AnimationsMap.end(); itb != ite; ++itb)
	{
		delete itb->second;
	}
	m_AnimationsMap.clear();
}

void CAnimationSet::AddAnimation(size_t aIndex, CAnimation* aAnimation)
{
	m_AnimationsMap[aIndex] = aAnimation;
}

CAnimation* CAnimationSet::GetAnimation(size_t aIndex) const
{
	return m_AnimationsMap.at(aIndex);
}