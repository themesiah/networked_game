#include "CameraController.h"

#include "imgui.h"

#include "Replication\LinkingContext.h"
#include "Replication\ReplicationManager.h"
#include "../Engine/Engine.h"

CCameraController::CCameraController() :
m_View(NULL)
, m_Follow(NULL)
, m_NetworkId(0)
{
	m_View = new sf::View(sf::Vector2f(683.f, 384.f), sf::Vector2f(1366.f, 768.f));
	m_LinkingContext = CEngine::GetInstance().GetReplicationManager().GetLinkingContext();
}

CCameraController::~CCameraController()
{
	delete m_View;
}

void CCameraController::Init(sf::RenderWindow* aWindow)
{
	m_Window = aWindow;
	aWindow->setView(*m_View);
}

void CCameraController::Update(const float& aDeltaTime)
{
	if (m_NetworkId != 0 && m_Follow == NULL)
	{
		GameObject* go = m_LinkingContext->GetGameObject(m_NetworkId);
		if (go != nullptr)
		{
			CPlayerControllerClient* lPlayer = static_cast<CPlayerControllerClient*>(go);
			if (lPlayer != nullptr)
			{
				SetFollow(lPlayer);
			}
		}
	}
	if (m_Follow != NULL)
	{
		m_View->setCenter(m_Follow->GetPosition());
		m_Window->setView(*m_View);
	}
}

void CCameraController::SetFollow(CPlayerControllerClient* aFollow)
{
	m_Follow = aFollow;
}

void CCameraController::SetFollow(uint32_t aNetworkId)
{
	m_NetworkId = aNetworkId;
}

void CCameraController::RenderImgui()
{
	ImGui::Text("Position is %fx %fy", m_View->getCenter().x, m_View->getCenter().y);
	ImGui::Text("Size is %fx%f", m_View->getSize().x, m_View->getSize().y);
	ImGui::Text("Viewport is %fx%f", m_View->getViewport().width, m_View->getViewport().height);
	if (m_Follow == NULL)
	{
		ImGui::Text("Not following any object");
	}
	else {
		ImGui::Text("Following object with network id %d", m_NetworkId);
	}
}