#pragma once

#include <SFML\System.hpp>
#include <unordered_set>

#include "Utils\Singleton.h"
#include "ResourcesManager.h"
#include "../Graphics/RenderManager.h"
#include "../Controllers/PlayerController.h"

#include "Socket\SocketUtil.h"
#include "Socket\TCPSocket.h"

#include "Serializer\PacketStream.h"

#define SEGMENT_SIZE 1500


#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
const C##Manager& Get##Manager() const { return *m_##Manager; } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Has##Manager() { return m_##Manager != nullptr; } \


class CActionManager;
class CTextureManager;
class CReplicationManager;
class CMovement;
class CNetworkManagerClient;
class CCameraController;
class CEngine : public base::utils::CSingleton<CEngine> {
public:
	virtual ~CEngine();
	void Init(sf::RenderWindow* aWindow);
	void ProcessInputs();
	void Update(float aDeltaTime);
	void Render(sf::RenderWindow* window);
	void ShowDebugHelpers();
	void Disconnect();
	std::unordered_set<GameObject*>* GetGameObjects()
	{
		return &m_GameObjects;
	}
	CMovement* GetMovement()
	{
		return m_Movement;
	}
	BUILD_GET_SET_ENGINE_MANAGER(ActionManager);
	BUILD_GET_SET_ENGINE_MANAGER(TextureManager);
	BUILD_GET_SET_ENGINE_MANAGER(SpriteManager);
	BUILD_GET_SET_ENGINE_MANAGER(RenderManager);
	BUILD_GET_SET_ENGINE_MANAGER(ReplicationManager);
	BUILD_GET_SET_ENGINE_MANAGER(NetworkManagerClient);
	BUILD_GET_SET_ENGINE_MANAGER(FontManager);
	BUILD_GET_SET_ENGINE_MANAGER(CameraController);
protected:
	CEngine();
	friend class base::utils::CSingleton<CEngine>;
private:
	void ManageObjectsDestroy();
	std::unordered_set<GameObject*> m_GameObjects;
	CMovement* m_Movement;
};