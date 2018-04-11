#pragma once

#ifndef H_GUI_MANAGER
#define H_GUI_MANAGER

#include <string>
#include <inttypes.h>
#include <SFML\Graphics.hpp>

#include "Serializer\OutputMemoryBitStream.h"

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
	bool DebugGUI(OutputMemoryBitStream& aOutputStream);
	void DisconnectionMessage(const std::string& aReason);
private:
	// Login
	void SetSelectedSprite();
	char m_Name[20];
	sf::Texture* m_SelectedTexture;
	sf::FloatRect m_TextureRect;
	uint16_t m_CurrentSelectedAnimation;

	// Debug
	char m_Secret[20];

	// Disconnection
};

#endif