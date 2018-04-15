#include "GUIManager.h"

#include "TextureManager.h"
#include "Engine.h"
#include "AnimationsetManager.h"
#include "../Graphics/Animation.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Replication\Packet.h"


CGUIManager::CGUIManager() :
m_CurrentSelectedAnimation(1)
{
	memset(m_Name, '\0', 20);
	memset(m_Secret, '\0', 20);
	SetSelectedSprite();
}

CGUIManager::~CGUIManager()
{

}

const uint16_t MAX_ANIMATION = 3;
CGUIManager::LoginData CGUIManager::LoginGUI()
{
	CTextureManager& lTextureManager = CEngine::GetInstance().GetTextureManager();
	CGUIManager::LoginData lLoginData;
	lLoginData.success = false;
	bool open = true;
	ImGui::Begin("LOGIN", &open, ImVec2(300.f, 100.f), 1.f);
	ImGui::InputText("Name", m_Name, 20);
	ImGui::Text("Character ID: %hu", m_CurrentSelectedAnimation);
	if (ImGui::Button("Previous")) {
		m_CurrentSelectedAnimation--;
		if (m_CurrentSelectedAnimation < 1)
		{
			m_CurrentSelectedAnimation = MAX_ANIMATION;
		}
		SetSelectedSprite();
	};
	ImGui::SameLine(); ImGui::Image(*m_SelectedTexture, m_TextureRect);
	ImGui::SameLine(); if(ImGui::Button("Next")) {
		m_CurrentSelectedAnimation++;
		if (m_CurrentSelectedAnimation > MAX_ANIMATION)
		{
			m_CurrentSelectedAnimation = 1;
		}
		SetSelectedSprite();
	};
	if (strcmp(m_Name, "") != 0) {
		if (ImGui::Button("Connect")) {
			lLoginData.name = m_Name;
			lLoginData.graphicId = m_CurrentSelectedAnimation;
			lLoginData.success = true;
		}
	}
	ImGui::End();
	return lLoginData;
}

void CGUIManager::SetSelectedSprite()
{
	auto animation = CEngine::GetInstance().GetAnimationsetManager().LoadAnimation(m_CurrentSelectedAnimation);
	m_SelectedTexture = animation->GetAnimation(1)->GetTexture();
	auto intrect = animation->GetAnimation(1)->GetFrame(0);
	m_TextureRect = sf::FloatRect(intrect);
}

bool CGUIManager::DebugGUI(OutputMemoryBitStream& aOutputStream)
{
	ImGui::Begin("DEBUG");
	ImGui::InputText("Secret", m_Secret, 20);
	if (ImGui::Button("Shutdown Server"))
	{
		OutputMemoryBitStream lOutput;
		aOutputStream.Serialize(PT_RPC, PACKET_BIT_SIZE);
		aOutputStream.Serialize<uint32_t>('SDWN');
		std::string secret(m_Secret);
		aOutputStream.Serialize(secret);
		ImGui::End();
		return true;
	}
	if (ImGui::Button("Reload Map"))
	{
		OutputMemoryBitStream lOutput;
		aOutputStream.Serialize(PT_RPC, PACKET_BIT_SIZE);
		aOutputStream.Serialize<uint32_t>('MRLD');
		std::string secret(m_Secret);
		aOutputStream.Serialize(secret);
		ImGui::End();
		return true;
	}
	ImGui::End();
	return false;
}

void CGUIManager::DisconnectionMessage(const std::string& aReason)
{
	bool open = true;
	ImGui::Begin("Disconnection", &open, ImVec2(600.f, 300.f), 1.f);
	ImGui::Text("Disconnected from server. Reason: %s", aReason.c_str());
	if (ImGui::Button("Exit"))
	{
		CEngine::GetInstance().Shutdown();
	}
	ImGui::End();
}