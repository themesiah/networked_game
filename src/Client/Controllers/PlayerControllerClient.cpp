#include "PlayerControllerClient.h"

#include "../Graphics/AnimatedSprite.h"
#include "../Graphics/AnimationSet.h"
#include "../Engine/Engine.h"
#include "../Graphics/RenderManager.h"
#include "../Network/NetworkManagerClient.h"
#include "../Engine/AnimationsetManager.h"


#include "imgui.h"
#include "imgui-SFML.h"

CPlayerControllerClient::CPlayerControllerClient() :
CPlayerController()
, m_pAnimatedSprite(NULL)
, m_pAnimationSet(NULL)
, m_Speed(150.f)
, m_CurrentAnimation(0)
, m_LastX(0.f)
, m_LastY(0.f)
, m_Timer(0.f)
, m_FirstUpdate(true)
, m_PacketTime(0.f)
{

}

CPlayerControllerClient::~CPlayerControllerClient()
{
	delete m_pAnimatedSprite;
}

void CPlayerControllerClient::Init() {
	m_pAnimatedSprite = new CAnimatedSprite();
	m_pAnimationSet = CEngine::GetInstance().GetAnimationsetManager().LoadAnimation(m_AnimationId);

	m_pAnimatedSprite->SetAnimation(m_pAnimationSet->GetAnimation(0));
	m_pAnimatedSprite->setOrigin(m_pAnimatedSprite->GetSpriteSize().x/2.f, m_pAnimatedSprite->GetSpriteSize().y/2.f);

	m_NameText;
	m_NameText.setFont(*CEngine::GetInstance().GetFontManager().Get("default"));
	m_NameText.setString("Unnamed");
	m_NameText.setCharacterSize(14);
	m_NameText.setFillColor(sf::Color::White);
	m_NameText.setOrigin(m_NameText.getGlobalBounds().width/2.f, m_NameText.getGlobalBounds().height/2.f);
}

void CPlayerControllerClient::Update(float aDeltaTime)
{
	float t = std::min(m_Timer / m_PacketTime, 1.f);
	if (m_FirstUpdate)
	{
		m_LastX = m_PosX;
		m_LastY = m_PosY;
	}
	m_FirstUpdate = false;

	float m_MovX = m_PosX - m_LastX;
	float m_MovY = m_PosY - m_LastY;

	size_t lNewAnimation = m_CurrentAnimation;
	if (m_MovX > 0) {
		lNewAnimation = PlayerAnimations::MOVE_RIGHT;
	}
	else if (m_MovX < 0) {
		lNewAnimation = PlayerAnimations::MOVE_LEFT;
	}
	else if (m_MovY > 0) {
		lNewAnimation = PlayerAnimations::MOVE_DOWN;
	}
	else if (m_MovY < 0) {
		lNewAnimation = PlayerAnimations::MOVE_UP;
	}

	if (lNewAnimation != m_CurrentAnimation || !m_pAnimatedSprite->IsPlaying())
	{
		m_CurrentAnimation = lNewAnimation;
		m_pAnimatedSprite->Play(m_pAnimationSet->GetAnimation(m_CurrentAnimation));
	}

	if (m_Timer > m_PacketTime)
	{
		m_pAnimatedSprite->Stop();
	}

	m_Timer += aDeltaTime;
	m_pAnimatedSprite->setPosition(m_LastX + m_MovX*t, m_LastY + m_MovY*t);
	m_NameText.setPosition(m_LastX + m_MovX*t, m_LastY + m_MovY*t - 30);

	m_pAnimatedSprite->Update(aDeltaTime);
	CEngine::GetInstance().GetRenderManager().Draw(m_pAnimatedSprite, 5);
	CEngine::GetInstance().GetRenderManager().Draw(&m_NameText, 6);
}

void CPlayerControllerClient::OnBeforeSerializeRead()
{
	m_PacketTime = CEngine::GetInstance().GetNetworkManagerClient().GetAverageTimeBetweenPackets();
	m_Timer = 0.f;
	m_LastX = m_PosX;
	m_LastY = m_PosY;
}

sf::Vector2f CPlayerControllerClient::GetPosition()
{
	return m_pAnimatedSprite->getPosition();
	//return sf::Vector2f(m_PosX, m_PosY);
}

void CPlayerControllerClient::SetName(const std::string& aName)
{
	m_NameText.setString(aName);
	m_NameText.setOrigin(m_NameText.getGlobalBounds().width / 2.f, m_NameText.getGlobalBounds().height / 2.f);
}

void CPlayerControllerClient::RenderImGui()
{
	if (ImGui::CollapsingHeader(m_NameText.getString().toAnsiString().c_str())) {
		ImGui::Text("PositionX: %f", m_PosX);
		ImGui::Text("PositionY: %f", m_PosY);
		ImGui::Text("Animation ID: %hu", m_AnimationId);
	}
}