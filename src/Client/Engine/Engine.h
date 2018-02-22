#pragma once

#include <SFML\System.hpp>

#include "Utils\Singleton.h"
#include "ResourcesManager.h"
#include "../Graphics/RenderManager.h"

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
const C##Manager& Get##Manager() const { return *m_##Manager; } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Has##Manager() { return m_##Manager != nullptr; } \

class CActionManager;
class CEngine : public base::utils::CSingleton<CEngine> {
public:
	virtual ~CEngine();
	void Init();
	void ProcessInputs();
	void Update(float aDeltaTime);
	void Render(sf::RenderWindow* window);
	void ShowDebugHelpers();

	BUILD_GET_SET_ENGINE_MANAGER(ActionManager);
	BUILD_GET_SET_ENGINE_MANAGER(TextureManager);
	BUILD_GET_SET_ENGINE_MANAGER(SpriteManager);
	BUILD_GET_SET_ENGINE_MANAGER(RenderManager);
protected:
	CEngine();
	friend class base::utils::CSingleton<CEngine>;
};