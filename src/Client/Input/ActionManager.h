#pragma once

#include <vector>

#include "Utils\TemplatedMap.h"
#include "ActionTrigger.h"
#include "InputManager.h"
#include "Utils\Defines.h"
#include <SFML\Window.hpp>


struct InputAction
{
	float value;
	bool active;
	int player;
	std::vector<ActionTrigger> triggers;
};

class CActionManager : public base::utils::CTemplatedMap<InputAction>
{
public:
	~CActionManager();
	void Update();
	void InitInputManager();
	bool HandleKeyboard(const sf::Event& msg);
	bool HandleMouse(const sf::Event& msg);
	void PreUpdate(bool windowIsActive);
	void PostUpdate();
	void LoadActions(const std::string filename);
	unsigned char GetKeyCode(const char *string);
	bool IsGamepadConnected(int player);
	GET_SET_REF(CInputManager, InputManager);

	bool KeyBecomesPressed(unsigned char keyCode);
	bool KeyBecomesReleased(unsigned char keyCode);

private:
	CInputManager m_InputManager;
};