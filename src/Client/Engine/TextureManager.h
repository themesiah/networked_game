#pragma once

#ifndef H_TEXTURE_MANAGER
#define H_TEXTURE_MANAGER

#include "ResourcesManager.h"

#include <SFML\Graphics\Texture.hpp>

class CTextureManager : public CResourcesManager<sf::Texture>
{
public:
	sf::Texture* LoadTexture(const std::string& aPath)
	{
		if (m_Resources.Exist(aPath)) {
			return Get(aPath);
		}
		else {
			sf::Texture* lNewTexture = new sf::Texture();
			if (lNewTexture->loadFromFile(aPath)) {
				Update(aPath, lNewTexture);
				return lNewTexture;
			}
			else {
				return nullptr;
			}
		}
	}
};

#endif