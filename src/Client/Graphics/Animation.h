#pragma once

#include <vector>
#include "Utils\Defines.h"

#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Texture.hpp>

class CAnimation {
public:
	CAnimation();
	virtual ~CAnimation();
	void AddFrame(const sf::IntRect& aRect);
	const sf::IntRect& GetFrame(size_t i);
	size_t GetSize() const;
	void SetTextureFrames(const int& rows, const int& columns, const int& count);
	GET_SET_PTR(sf::Texture, Texture);
private:
	std::vector<sf::IntRect> m_Rectangles;
	sf::Texture* m_pTexture;
};