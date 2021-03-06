#pragma once

#ifndef H_CAMERA_CONTROLLER
#define H_CAMERA_CONTROLLER

#include <inttypes.h>

#include "SFML\Graphics.hpp"

class LinkingContext;
class CPlayerControllerClient;
class CCameraController
{
public:
	CCameraController();
	virtual ~CCameraController();
	void Init(sf::RenderWindow* aWindow);
	void Update(const float& aDeltaTime);
	void SetFollow(CPlayerControllerClient* aFollow);
	void SetFollow(uint32_t aNetworkId);
	void RenderImgui();
private:
	sf::View* m_View;
	CPlayerControllerClient* m_Follow;
	sf::RenderWindow* m_Window;
	uint32_t m_NetworkId;
	LinkingContext* m_LinkingContext;
};

#endif