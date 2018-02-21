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

#define TARGET_FPS 60.0f

int main() {
	// GENERAL VARIABLES
	float lSecondsToEndFrame = 1.0f / TARGET_FPS;
	sf::Clock mMainClock;
	bool fHandled = false;

	// INIT WINDOW
	sf::RenderWindow window(sf::VideoMode(1366, 768), "SFML works!");
	window.setVerticalSyncEnabled(true);

	// INIT ENGINE
	CEngine& lEngine = CEngine::GetInstance();

	CActionManager* lActionManager = new CActionManager();
	lActionManager->InitInputManager();
	lActionManager->LoadActions("actions.xml");
	lEngine.SetActionManager(lActionManager);

	CTextureManager* lTextureManager = new CTextureManager();
	lTextureManager->SetOnDestructor(DestroyOnDestructor);
	lEngine.SetTextureManager(lTextureManager);

	CSpriteManager* lSpriteManager = new CSpriteManager();
	lSpriteManager->SetOnDestructor(DestroyOnDestructor);
	lEngine.SetSpriteManager(lSpriteManager);
	
	// INIT NETWORK

	// MAIN LOOP
	while (window.isOpen())
	{
		// Reestart the main FPS clock
		float lDeltaTime = mMainClock.restart().asSeconds();

		// Poll SFML Events
		sf::Event event;
		lEngine.GetActionManager().PreUpdate(window.hasFocus());
		fHandled = false;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				// Handle disconnection here
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
		lEngine.Update(lDeltaTime);

		// Check game finish
		if (lEngine.GetActionManager().Exist("EXIT") && lEngine.GetActionManager().Get("EXIT")->active) {
			// Handle disconnection here
			window.close();
		}

		// Render
		window.clear();
		lEngine.Render();
		window.display();

		// Wait for end of frame
		while (mMainClock.getElapsedTime().asSeconds() < lSecondsToEndFrame);
	}
	return 0;
}