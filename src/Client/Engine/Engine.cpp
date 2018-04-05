#include "Engine.h"

#include <iostream>
#include <fstream>

#include "../Input/ActionManager.h"
#include "Replication\ReplicationManager.h"
#include "TextureManager.h"
#include "../Network/NetworkManagerClient.h"
#include "../Graphics/CameraController.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Replication\ObjectCreationRegistry.h"

#include "Socket\SocketAddress.h"
#include "Socket\SocketAddressFactory.h"


#include "CommonClasses\Movement.h"
#include "Replication\Packet.h"

#include "../Other/ImGUILog.h"

#include "SFML\Graphics\Font.hpp"

CEngine::CEngine()
{
	m_Movement = new CMovement();
}

CEngine::~CEngine()
{
	delete m_ActionManager;
	delete m_RenderManager;
	delete m_TextureManager;
	delete m_SpriteManager;
	delete m_ReplicationManager;
	delete m_FontManager;
	delete m_NetworkManagerClient;
	delete m_CameraController;
	delete m_Movement;

	for (std::unordered_set<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		delete *it;
	}
	m_GameObjects.clear();
}

void CEngine::Init(sf::RenderWindow* aWindow)
{
	bool success = true;
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

	CFontManager* lFontManager = new CFontManager();
	lFontManager->SetOnDestructor(DestroyOnDestructor);
	sf::Font* lDefaultFont = new sf::Font();
	lDefaultFont->loadFromFile("Data/BebasNeue.otf");
	lFontManager->Update("default", lDefaultFont);
	SetFontManager(lFontManager);

	CRenderManager* lRenderManager = new CRenderManager();
	lRenderManager->Init();
	SetRenderManager(lRenderManager);

	CReplicationManager* lReplicationManager = new CReplicationManager();
	SetReplicationManager(lReplicationManager);

	CNetworkManagerClient* lNetworkManagerClient = new CNetworkManagerClient();
	success = lNetworkManagerClient->Init(0);
	success = lNetworkManagerClient->Connect();
	SetNetworkManagerClient(lNetworkManagerClient);

	CCameraController* lCameraController = new CCameraController();
	lCameraController->Init(aWindow);
	SetCameraController(lCameraController);
}

void CEngine::ProcessInputs()
{
	m_ActionManager->Update();
}

void CEngine::Update(float aDeltaTime)
{
	// MANAGE INPUT
	float x = 0.f, y = 0.f;
	if (CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->active) {
		x = CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->value;
	}
	else if (CEngine::GetInstance().GetActionManager().Get("VERTICAL")->active) {
		y = CEngine::GetInstance().GetActionManager().Get("VERTICAL")->value;
	}
	m_Movement->SetMovement(x, y);
	// END MANAGE INPUT

	m_NetworkManagerClient->UpdateSendingSockets(aDeltaTime);
	m_NetworkManagerClient->UpdateReceivingSockets(aDeltaTime);
	m_NetworkManagerClient->UpdatePackets(aDeltaTime);

	// First destroy. Then update the rest.
	ManageObjectsDestroy();
	for (GameObject* go : m_GameObjects)
	{
		go->Update(aDeltaTime);
	}

	m_CameraController->Update(aDeltaTime);
}

void CEngine::ManageObjectsDestroy()
{
	std::vector<GameObject*> toDestroy;
	for (auto go : m_GameObjects)
	{
		if (go->GetDirty() == GameObject::DirtyType::PREPARED_TO_DESTROY)
		{
			toDestroy.push_back(go);
		}
	}

	for (auto go : toDestroy)
	{
		auto goit = std::find(m_GameObjects.begin(), m_GameObjects.end(), go);
		m_GameObjects.erase(goit);
		delete go;
	}
}

void CEngine::Render(sf::RenderWindow* window)
{
	m_RenderManager->Render(window);
}

void CEngine::ShowDebugHelpers()
{
	IMLOG_DRAW;
	ImGui::Begin("Debug"); // begin window
	ImGui::Text("FPS: %d", (int)ImGui::GetIO().Framerate);
	if (ImGui::CollapsingHeader("Network Manager")) {
		ImGui::Indent();
		ImGui::PushID("NETWORKMANAGER");
		m_NetworkManagerClient->RenderImGui();
		ImGui::PopID();
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Game Objects")) {
		ImGui::Indent();
		ImGui::PushID("ENGINEINFO");
		for (GameObject* go : m_GameObjects)
		{
			go->RenderImGui();
		}
		ImGui::PopID();
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Camera Controller")) {
		ImGui::Indent();
		ImGui::PushID("CAMERACONTROLLER");
		m_CameraController->RenderImgui();
		ImGui::PopID();
		ImGui::Unindent();
	}
	
	ImGui::End();
}

void CEngine::Disconnect()
{
	m_NetworkManagerClient->ManageDisconnection();
}