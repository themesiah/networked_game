#include "PlayerController.h"

#include "../Graphics/AnimatedSprite.h"
#include "../Graphics/AnimationSet.h"
#include "../Graphics/Animation.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Engine.h"
#include "../Input/ActionManager.h"
#include "../Graphics/RenderManager.h"


#include "imgui.h"
#include "imgui-SFML.h"

CPlayerController::CPlayerController() :
m_PosX(300.f)
, m_PosY(300.f)
, m_pAnimatedSprite(new CAnimatedSprite())
, m_pAnimationSet(new CAnimationSet())
, m_Speed(150.f)
, m_CurrentAnimation(0)

{
	
}

CPlayerController::~CPlayerController()
{
	delete m_pAnimatedSprite;
	delete m_pAnimationSet;
}

void CPlayerController::Init() {
	sf::Texture* lTexture = CEngine::GetInstance().GetTextureManager().LoadTexture("Data/player.png");
	const unsigned int width = 32;
	const unsigned int height = 41;
	// Animation up
	CAnimation* lAnimationUp = new CAnimation();
	lAnimationUp->SetTexture(lTexture);
	lAnimationUp->AddFrame(sf::IntRect(0, height * 3, width, height));
	lAnimationUp->AddFrame(sf::IntRect(width, height * 3, width, height));
	lAnimationUp->AddFrame(sf::IntRect(width * 2, height * 3, width, height));
	m_pAnimationSet->AddAnimation(PlayerAnimations::MOVE_UP, lAnimationUp);

	// Animation down
	CAnimation* lAnimationDown = new CAnimation();
	lAnimationDown->SetTexture(lTexture);
	lAnimationDown->AddFrame(sf::IntRect(0, 0, width, height));
	lAnimationDown->AddFrame(sf::IntRect(width, 0, width, height));
	lAnimationDown->AddFrame(sf::IntRect(width * 2, 0, width, height));
	m_pAnimationSet->AddAnimation(PlayerAnimations::MOVE_DOWN, lAnimationDown);

	// Animation right
	CAnimation* lAnimationRight = new CAnimation();
	lAnimationRight->SetTexture(lTexture);
	lAnimationRight->AddFrame(sf::IntRect(0, height * 2, width, height));
	lAnimationRight->AddFrame(sf::IntRect(width, height * 2, width, height));
	lAnimationRight->AddFrame(sf::IntRect(width * 2, height * 2, width, height));
	m_pAnimationSet->AddAnimation(PlayerAnimations::MOVE_RIGHT, lAnimationRight);

	// Animation left
	CAnimation* lAnimationLeft = new CAnimation();
	lAnimationLeft->SetTexture(lTexture);
	lAnimationLeft->AddFrame(sf::IntRect(0, height, width, height));
	lAnimationLeft->AddFrame(sf::IntRect(width, height, width, height));
	lAnimationLeft->AddFrame(sf::IntRect(width * 2, height, width, height));
	m_pAnimationSet->AddAnimation(PlayerAnimations::MOVE_LEFT, lAnimationLeft);

	m_pAnimatedSprite->SetAnimation(lAnimationDown);

	
}

void CPlayerController::Update(float aDeltaTime)
{
	float x = 0.f;
	float y = 0.f;
	size_t lNewAnimation = m_CurrentAnimation;
	if (CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->active) {
		x = CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->value;
		if (x > 0) {
			lNewAnimation = PlayerAnimations::MOVE_RIGHT;
		}
		else if (x < 0) {
			lNewAnimation = PlayerAnimations::MOVE_LEFT;
		}
	}
	else if (CEngine::GetInstance().GetActionManager().Get("VERTICAL")->active) {
		y = CEngine::GetInstance().GetActionManager().Get("VERTICAL")->value;
		if (y > 0) {
			lNewAnimation = PlayerAnimations::MOVE_DOWN;
		}
		else if (y < 0) {
			lNewAnimation = PlayerAnimations::MOVE_UP;
		}
	}

	if (lNewAnimation != m_CurrentAnimation)
	{
		m_CurrentAnimation = lNewAnimation;
		m_pAnimatedSprite->Play(m_pAnimationSet->GetAnimation(m_CurrentAnimation));
	}

	if (x == 0.f && y == 0.f)
	{
		m_pAnimatedSprite->Stop();
	}


	/*m_PosX += x * aDeltaTime * m_Speed;
	m_PosY += y * aDeltaTime * m_Speed;*/

	m_pAnimatedSprite->setPosition(m_PosX, m_PosY);
	m_pAnimatedSprite->Update(aDeltaTime);
	CEngine::GetInstance().GetRenderManager().Draw(m_pAnimatedSprite, 5);
}