#pragma once

#include "Replication\GameObject.h"

#include "SFML\System\Clock.hpp"
#include "SFML\Graphics\Text.hpp"

class CAnimatedSprite;
class CAnimationSet;

class OutputMemoryBitStream;
class InputMemoryBitStream;
class CPlayerController : public GameObject
{
public:
	enum PlayerAnimations {
		MOVE_UP = 0,
		MOVE_DOWN = 1,
		MOVE_RIGHT = 2,
		MOVE_LEFT = 3
	};
	CPlayerController();
	virtual ~CPlayerController();
	void Init();
	void Update(float aDeltaTime) override;
	virtual void RenderImGui() override;
	
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("m_PosX", OffsetOf(CPlayerController, m_PosX), -1000.0f, 0.1f),
			MemberVariable("m_PosY", OffsetOf(CPlayerController, m_PosY), -1000.0f, 0.1f)
		}
		);
	}
	CLASS_IDENTIFICATION('CPCT', CPlayerController);
protected:
	virtual void OnBeforeSerializeRead() override;
private:
	CAnimatedSprite* m_pAnimatedSprite;
	sf::Text* m_NameText;
	CAnimationSet* m_pAnimationSet;
	float m_PosX;
	float m_PosY;
	float m_LastX;
	float m_LastY;
	float m_Speed;
	float m_Timer;
	size_t m_CurrentAnimation;
	bool m_FirstUpdate;
	sf::Clock m_Clock;
	float m_PacketTime;
};
