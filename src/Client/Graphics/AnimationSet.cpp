#include "AnimationSet.h"

#include "Animation.h"

#include <rapidjson\document.h>

#include "Utils\FileUtils.h"
#include "../Engine/Engine.h"
#include "../Engine/TextureManager.h"

using namespace rapidjson;

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

bool CAnimationSet::LoadResource(const std::string& aResourceName)
{
	std::string jsonData = base::utils::GetTextFromFile(aResourceName);
	Document document;
	document.Parse(jsonData.c_str(), jsonData.size());
	const Value& animations = document["animations"];
	SizeType animationsSize = animations.Size();
	CEngine& lEngine = CEngine::GetInstance();
	for (SizeType i = 0; i < animationsSize; ++i)
	{
		CAnimation* lAnimation = new CAnimation();
		uint16_t lTextureId = animations[i]["texture"].GetInt();
		std::string lTextureName = lEngine.GetResourceTable(CResourcesTable::TEXTURES)->GetResourcePath(lTextureId);
		lAnimation->SetTexture(lEngine.GetTextureManager().LoadTexture(lTextureName));
		const Value& frames = animations[i]["frames"];
		SizeType framesSize = frames.Size();
		for (SizeType j = 0; j < framesSize; ++j)
		{
			const Value& frame = frames[j];
			lAnimation->AddFrame(sf::IntRect(frame["left"].GetInt(), frame["top"].GetInt(), frame["width"].GetInt(), frame["height"].GetInt()));
		}
		AddAnimation(i, lAnimation);
	}
	return true;
}