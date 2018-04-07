#pragma once

#ifndef H_GUI_MANAGER
#define H_GUI_MANAGER

#include <string>
#include <inttypes.h>
#include <SFML\Graphics.hpp>

class CGUIManager
{
public:
	struct LoginData {
		bool success;
		std::string name;
		uint16_t graphicId;
	};
	CGUIManager();
	virtual ~CGUIManager();
	LoginData LoginGUI();
private:
	void SetSelectedSprite();
	char m_Name[50];
	sf::Texture* m_SelectedTexture;
	sf::FloatRect m_TextureRect;
	uint16_t m_CurrentSelectedAnimation;
};

#endif