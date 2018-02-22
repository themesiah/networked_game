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

void CEngine::Init()
{
	CActionManager* lActionManager = new CActionManager();
	lActionManager->InitInputManager();
	lActionManager->LoadActions("actions.xml");
	SetActionManager(lActionManager);

	CTextureManager* lTextureManager = new CTextureManager();
	lTextureManager->SetOnDestructor(DestroyOnDestructor);
	SetTextureManager(lTextureManager);

	CSpriteManager* lSpriteManager = new CSpriteManager();
	lSpriteManager->SetOnDestructor(DestroyOnDestructor);
	SetSpriteManager(lSpriteManager);

	CRenderManager* lRenderManager = new CRenderManager();
	lRenderManager->Init();
	SetRenderManager(lRenderManager);
}

void CEngine::ProcessInputs()
{
	m_ActionManager->Update();
}

void CEngine::Update(float aDeltaTime)
{
	
}

void CEngine::Render(sf::RenderWindow* window)
{
	m_RenderManager->Render(window);
}

void CEngine::ShowDebugHelpers()
{
	ImGui::Begin("Sample window"); // begin window
	if (ImGui::Button("Hello, im a button")) {

	}
	ImGui::End();
}