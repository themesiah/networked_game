#pragma once

#ifndef H_PLAYER_CONTROLLER_CLIENT
#define H_PLAYER_CONTROLLER_CLIENT

#include "Replication\GameObject.h"
#include "CommonClasses\PlayerController.h"

#include "SFML\System\Clock.hpp"
#include "SFML\Graphics\Text.hpp"

class CAnimatedSprite;
class CAnimationSet;

class OutputMemoryBitStream;
class InputMemoryBitStream;
class CPlayerControllerClient : public CPlayerController
{
public:
	enum PlayerAnimations {
		MOVE_UP = 0,
		MOVE_DOWN = 1,
		MOVE_RIGHT = 2,
		MOVE_LEFT = 3
	};
	CPlayerControllerClient();
	virtual ~CPlayerControllerClient();
	virtual void Init() override;
	void Update(float aDeltaTime) override;
	virtual void RenderImGui() override;

	sf::Vector2f GetPosition();
	void SetName(const std::string& aName);
	CLASS_IDENTIFICATION('CPCT', CPlayerControllerClient);
protected:
	virtual void OnBeforeSerializeRead() override;
private:
	CAnimatedSprite* m_pAnimatedSprite;
	sf::Text m_NameText;
	CAnimationSet* m_pAnimationSet;
	float m_LastX;
	float m_LastY;
	float m_Speed;
	float m_Timer;
	size_t m_CurrentAnimation;
	bool m_FirstUpdate;
	sf::Clock m_Clock;
	float m_PacketTime;
};

#endif