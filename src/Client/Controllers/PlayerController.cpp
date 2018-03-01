#include "PlayerController.h"

#include "../Graphics/AnimatedSprite.h"
#include "../Graphics/AnimationSet.h"
#include "../Graphics/Animation.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Engine.h"
#include "../Input/ActionManager.h"
#include "../Graphics/RenderManager.h"


#include "Common\SocketAddress.h"
#include "Common\SocketAddressFactory.h"

#include "Serializer\OutputMemoryBitStream.h"
#include "Serializer\InputMemoryBitStream.h"

#include "imgui.h"
#include "imgui-SFML.h"

CPlayerController::CPlayerController() :
m_PosX(300.f)
, m_PosY(300.f)
, m_pAnimatedSprite(new CAnimatedSprite())
, m_pAnimationSet(new CAnimationSet())
, m_Speed(150.f)
, m_CurrentAnimation(0)
, m_Socket(NULL)
{
}

CPlayerController::~CPlayerController()
{
	delete m_pAnimatedSprite;
	delete m_pAnimationSet;
}

void CPlayerController::Init() {
	SET_REFLECTION_DATA(CPlayerController);
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

	SocketUtil::InitSockets();
	m_Socket = SocketUtil::CreateTCPSocket(INET);
	SocketAddressPtr sendingAddress = SocketAddressFactory::CreateIPv4FromString("localhost:48000");
	SocketAddress receivingAddress(INADDR_ANY, 0);
	if (m_Socket->Bind(receivingAddress) == NO_ERROR)
	{
		if (m_Socket->Connect(*sendingAddress.get()) == NO_ERROR)
		{
			m_Socket->SetNonBlockingMode(false);
		}
	}
	m_ReadBlockSockets.push_back(m_Socket);
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


	m_PosX += x * aDeltaTime * m_Speed;
	m_PosY += y * aDeltaTime * m_Speed;

	SocketUpdate(aDeltaTime);

	//m_pAnimatedSprite->setPosition(m_PosX, m_PosY);
	m_pAnimatedSprite->Update(aDeltaTime);
	CEngine::GetInstance().GetRenderManager().Draw(m_pAnimatedSprite, 5);
}

void CPlayerController::SocketUpdate(float aDeltaTime)
{
	
	MemoryStream *output = Serialize();
	int sent = m_Socket->Send(output->GetBufferPtr(), output->GetByteLength());
	
	std::vector<TCPSocketPtr> readableSockets;
	if (SocketUtil::Select(&m_ReadBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr)) {
		for (const TCPSocketPtr& socket : readableSockets)
		{
			char segment[1024];
			FD_ZERO(segment);
			int dataReceived = socket->Receive(segment, 1024);
			if (dataReceived > 0) {
				InputMemoryBitStream *input = new InputMemoryBitStream(segment, dataReceived);
				Unserialize(input);
				m_pAnimatedSprite->setPosition(m_PosX, m_PosY);
			}
		}
	}

	ImGui::Begin("TEST");
	ImGui::SliderFloat("Pos X", &m_PosX, -1000.0f, 1000.0f);
	ImGui::SliderFloat("Pos Y", &m_PosY, -1000.0f, 1000.0f);
	ImGui::End();
}