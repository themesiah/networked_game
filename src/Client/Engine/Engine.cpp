#include "Engine.h"

#include <iostream>
#include <fstream>

#include "../Input/ActionManager.h"
#include "Replication\ReplicationManager.h"
#include "TextureManager.h"
#include "../Network/NetworkManagerClient.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Replication\ObjectCreationRegistry.h"

#include "Socket\SocketAddress.h"
#include "Socket\SocketAddressFactory.h"


#include "Movement.h"
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
	delete m_Movement;

	for (std::unordered_set<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		delete *it;
	}
	m_GameObjects.clear();
}

void CEngine::Init()
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
}

void CEngine::ProcessInputs()
{
	m_ActionManager->Update();
}

void CEngine::Update(float aDeltaTime)
{
	// MANAGE INPUT
	if (CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->active) {
		m_Movement->inputX = CEngine::GetInstance().GetActionManager().Get("HORIZONTAL")->value;
	}
	else if (CEngine::GetInstance().GetActionManager().Get("VERTICAL")->active) {
		m_Movement->inputY = CEngine::GetInstance().GetActionManager().Get("VERTICAL")->value;
	}
	// END MANAGE INPUT

	m_NetworkManagerClient->UpdateSendingSockets(aDeltaTime);
	m_NetworkManagerClient->UpdateReceivingSockets(aDeltaTime);
	m_NetworkManagerClient->UpdatePackets(aDeltaTime);

	for (GameObject* go : m_GameObjects)
	{
		go->Update(aDeltaTime);
	}
}

void CEngine::Render(sf::RenderWindow* window)
{
	m_RenderManager->Render(window);
}

void CEngine::ShowDebugHelpers()
{
	IMLOG_DRAW;
	ImGui::Begin("Sample window"); // begin window
	ImGui::Text("FPS: %d", (int)ImGui::GetIO().Framerate);
	if (ImGui::CollapsingHeader("Network Manager")) {
		ImGui::Indent();
		ImGui::PushID("NETWORKMANAGER");
		m_NetworkManagerClient->RenderImGui();
		ImGui::PopID();
		ImGui::Unindent();
	}
	ImGui::End();
}

void CEngine::Disconnect()
{
	m_NetworkManagerClient->ManageDisconnection();
}