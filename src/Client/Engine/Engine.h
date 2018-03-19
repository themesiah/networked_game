#pragma once

#include <SFML\System.hpp>
#include <unordered_set>

#include "Utils\Singleton.h"
#include "ResourcesManager.h"
#include "../Graphics/RenderManager.h"
#include "../Controllers/PlayerController.h"

#include "Common\SocketUtil.h"
#include "Common\TCPSocket.h"

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
class CEngine : public base::utils::CSingleton<CEngine> {
public:
	virtual ~CEngine();
	void Init();
	void ProcessInputs();
	void Update(float aDeltaTime);
	void Render(sf::RenderWindow* window);
	void ShowDebugHelpers();
	void Disconnect();

	BUILD_GET_SET_ENGINE_MANAGER(ActionManager);
	BUILD_GET_SET_ENGINE_MANAGER(TextureManager);
	BUILD_GET_SET_ENGINE_MANAGER(SpriteManager);
	BUILD_GET_SET_ENGINE_MANAGER(RenderManager);
	BUILD_GET_SET_ENGINE_MANAGER(ReplicationManager);
protected:
	CEngine();
	void InitReflection();
	void InitNetwork();
	void UpdateNetwork(float aDeltaTime);
	friend class base::utils::CSingleton<CEngine>;
	std::unordered_set<GameObject*> m_GameObjects;

	TCPSocketPtr m_Socket;
	OutputMemoryBitStream *m_OutputMs;
	InputMemoryBitStream *m_InputMs;
	CMovement* m_Movement;
	PacketStream m_PacketStream;
};