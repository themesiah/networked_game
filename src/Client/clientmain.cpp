/*#include <iostream>

#include "Common\TCPSocket.h"
#include "Common\SocketUtil.h"
#include "Common\SocketAddressFactory.h"

#define GOOD_SEGMENT_SIZE 1024

int main()
{
	SocketUtil::InitSockets();
	TCPSocketPtr socket = SocketUtil::CreateTCPSocket(INET);
	SocketAddressPtr sendingAddress = SocketAddressFactory::CreateIPv4FromString("localhost:48000");
	SocketAddress receivingAddress(INADDR_ANY, 0);
	if (socket->Bind(receivingAddress) != NO_ERROR)
	{
		return -1;
	}
	if (socket->Connect(*sendingAddress.get()) != NO_ERROR) {
		return -1;
	}
	socket->SetNonBlockingMode(true);
	std::string segment;
	while (strcmp(segment.c_str(), "END") != 0)
	{
		char segment[GOOD_SEGMENT_SIZE];
		std::cin.getline(segment, sizeof(segment));
		std::cout << "Sent message: " << segment << std::endl;
		socket->Send(segment, strlen(segment));
	}


	return 0;
}*/

#include "Engine\Engine.h"
#include "Input\ActionManager.h"
#include "Engine\ResourcesManager.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Graphics\AnimatedSprite.h"
#include "Graphics\Animation.h"

#include <SFML\Graphics.hpp>

#ifdef _DEBUG
#include "Utils\MemLeaks\MemLeaks.h"
#endif

#define TARGET_FPS 60.0f

int MainGame() {
	// GENERAL VARIABLES
	float lSecondsToEndFrame = 1.0f / TARGET_FPS;
	sf::Clock mMainClock;
	sf::Clock mImguiClock;
	bool fHandled = false;

	// INIT WINDOW
	sf::RenderWindow window(sf::VideoMode(1366, 768), "Banned Lands");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	// INIT ENGINE
	CEngine& lEngine = CEngine::GetInstance();
	lEngine.Init(&window);

	// INIT NETWORK


	// MAIN LOOP
	mImguiClock.restart();
	while (window.isOpen() && !lEngine.IsFinished())
	{
		// Reestart the main FPS clock
		float lDeltaTime = mMainClock.restart().asSeconds();

		// Poll SFML Events
		sf::Event event;
		lEngine.GetActionManager().PreUpdate(window.hasFocus());
		fHandled = false;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed) {
				// Handle disconnection here
				lEngine.Disconnect();
				window.close();
			}

			if (event.type >= sf::Event::EventType::MouseWheelMoved && sf::Event::EventType::MouseLeft) {
				fHandled = lEngine.GetActionManager().HandleMouse(event);
			}
			if (event.type >= sf::Event::EventType::TextEntered && event.type <= sf::Event::EventType::KeyReleased) {
				fHandled = lEngine.GetActionManager().HandleKeyboard(event);
			}
		}
		lEngine.GetActionManager().PostUpdate();

		// Main Update
		lEngine.ProcessInputs();
		ImGui::SFML::Update(window, mImguiClock.restart());
		lEngine.Update(lDeltaTime);
		lEngine.ShowDebugHelpers();


		// Check game finish
		if (lEngine.GetActionManager().Exist("EXIT") && lEngine.GetActionManager().Get("EXIT")->active) {
			// Handle disconnection here
			window.close();
		}

		// Render
		window.clear();
		lEngine.Render(&window);
		ImGui::SFML::Render(window);
		window.display();

		// Wait for end of frame
		while (mMainClock.getElapsedTime().asSeconds() < lSecondsToEndFrame);
	}
	ImGui::SFML::Shutdown();
	return 0;
}

int main() {
#ifdef _DEBUG
	MemLeaks::MemoryBegin();
	//_CrtSetBreakAlloc(4073);
#endif
	MainGame();
#ifdef _DEBUG
	MemLeaks::MemoryEnd();
#endif
	return 0;
}