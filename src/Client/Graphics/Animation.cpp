#include "Animation.h"

#include <SFML\Graphics.hpp>

CAnimation::CAnimation() : m_pTexture(NULL)
{
	
}

CAnimation::~CAnimation()
{
	m_Rectangles.clear();
}

void CAnimation::AddFrame(const sf::IntRect& aRect)
{
	m_Rectangles.push_back(aRect);
}

const sf::IntRect& CAnimation::GetFrame(size_t i)
{
	return m_Rectangles[i];
}

size_t CAnimation::GetSize() const
{
	return m_Rectangles.size();
};

void CAnimation::SetTextureFrames(const int& rows, const int& columns, const int& count)
{
	sf::Vector2u lSize = m_pTexture->getSize();
	sf::Vector2u lFrameSize;
	lFrameSize.x = lSize.x / columns;
	lFrameSize.y = lSize.y / rows;

	for (unsigned int i = 0; i < count; ++i) {
		AddFrame(sf::IntRect(i*lFrameSize.x, i*lFrameSize.y, lFrameSize.x, lFrameSize.y));
	}
}