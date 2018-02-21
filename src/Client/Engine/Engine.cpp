#include "Engine.h"

#include "../Input/ActionManager.h"

#include "imgui.h"
#include "imgui-SFML.h"

CEngine::CEngine()
{

}

CEngine::~CEngine()
{

}

void CEngine::ProcessInputs()
{
	m_ActionManager->Update();
}

void CEngine::Update(float aDeltaTime)
{
	
}

void CEngine::Render(const sf::RenderWindow& window)
{
	
}

void CEngine::ShowDebugHelpers()
{
	ImGui::Begin("Sample window"); // begin window
	if (ImGui::Button("Hello, im a button")) {

	}
	ImGui::End();
}