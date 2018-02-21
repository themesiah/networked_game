#include "ActionManager.h"

#include "Debug\CustomAssert.h"

#include "XML\XML.h"
#include "Paths.h"

CActionManager::~CActionManager()
{

}

void CActionManager::InitInputManager()
{
	m_InputManager = CInputManager();
	m_InputManager.Init();
}

void CActionManager::PreUpdate(bool windowIsActive)
{
	m_InputManager.PreUpdate(windowIsActive);
}

void CActionManager::PostUpdate()
{
	m_InputManager.PostUpdate();
}

bool CActionManager::HandleKeyboard(const sf::Event& msg)
{
	return m_InputManager.HandleKeyboard(msg);
}

bool CActionManager::HandleMouse(const sf::Event& msg)
{
	return m_InputManager.HandleMouse(msg);
}

void CActionManager::Update()
{
	m_InputManager.Update();

	for (auto &actionIt : m_ResourcesMap)
	{
		InputAction *action = actionIt.second;
		action->active = false;
		action->value = 0;
		action->player = 0;
		for (ActionTrigger& trigger : action->triggers)
		{
			switch (trigger.type)
			{
			case ActionTrigger::KEYBOARD:
				switch (trigger.keyboard.actionType)
				{
				case ActionTrigger::IsPressed:
					if (m_InputManager.IsKeyPressed(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::IsReleased:
					if (!m_InputManager.IsKeyPressed(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::BecomesPressed:
					if (m_InputManager.KeyBecomesPressed(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::BecomesReleased:
					if (m_InputManager.KeyBecomesReleased(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				}
				break;
			case ActionTrigger::MOUSE:
				if (action->value == 0.0f)
				{
					float value = (float)m_InputManager.GetMouseAxis(trigger.mouse.axis);
					float comparingValue = value;
					if (comparingValue < 0) comparingValue *= -1;
					if (value != 0.0f)
					{
						value = value;
					}
					if (trigger.mouse.invert == true) {
						value = -value;
					}
					if (trigger.mouse.sensitivity) {
						value *= trigger.mouse.sensitivity;
					}
					bool active = (trigger.mouse.geThreshold) ?
						(comparingValue >= trigger.mouse.threshold) : (comparingValue < trigger.mouse.threshold);
					if (fabs(value) > fabs(action->value))
					{
						if (active || !action->active)
						{
							action->value = value;
							action->active = active;
						}
					}
				}
				break;
			case ActionTrigger::MOUSE_BUTTON:
				switch (trigger.mouseButton.actionType)
				{
				case ActionTrigger::IsPressed:
					if (m_InputManager.IsMouseButtonPressed(trigger.mouseButton.button))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::IsReleased:
					if (!m_InputManager.IsMouseButtonPressed(trigger.mouseButton.button))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::BecomesPressed:
					if (m_InputManager.MouseButtonBecomesPressed(trigger.mouseButton.button))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::BecomesReleased:
					if (m_InputManager.MouseButtonBecomesReleased(trigger.mouseButton.button))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				}
				break;
			case ActionTrigger::GAMEPAD_BUTTON:
				switch (trigger.gamepadButton.actionType)
				{
				case ActionTrigger::IsPressed:
					if (m_InputManager.IsGamepadButtonPressed(trigger.gamepadButton.button, trigger.gamepadButton.player))
					{
						action->active = true;
						action->value = trigger.gamepadButton.value;
						action->player = trigger.gamepadButton.player;
					}
					break;
				case ActionTrigger::IsReleased:
					if (!m_InputManager.IsGamepadButtonPressed(trigger.gamepadButton.button, trigger.gamepadButton.player))
					{
						action->active = true;
						action->value = trigger.gamepadButton.value;
						action->player = trigger.gamepadButton.player;
					}
					break;
				case ActionTrigger::BecomesPressed:
					if (m_InputManager.GamepadButtonBecomesPressed(trigger.gamepadButton.button, trigger.gamepadButton.player))
					{
						action->active = true;
						action->value = trigger.gamepadButton.value;
						action->player = trigger.gamepadButton.player;
					}
					break;
				case ActionTrigger::BecomesReleased:
					if (m_InputManager.GamepadButtonBecomesReleased(trigger.gamepadButton.button, trigger.gamepadButton.player))
					{
						action->active = true;
						action->value = trigger.gamepadButton.value;
						action->player = trigger.gamepadButton.player;
					}
					break;
				}
				break;
			case ActionTrigger::GAMEPAD:
				if (action->value == 0.0f)
				{
					float value;
					value = m_InputManager.GetGamepadAxis(trigger.gamepad.axis, trigger.gamepad.player);
					action->player = trigger.gamepad.player;
					if (trigger.gamepad.invert == true) {
						value = -value;
					}
					if (trigger.gamepad.sensitivity) {
						value *= trigger.gamepad.sensitivity;
					}
					if (value != 0.0f)
					{
						action->value = value;
						action->active = true;
					}
				}

				break;

			}
		}
	}
}

unsigned char CActionManager::GetKeyCode(const char *string)
{
	if (string == NULL) return -1;
	if (strlen(string) == 1 && ((string[0] >= 'A' && string[0] <= 'Z') || (string[0] >= '0' && string[0] <= '9')))
	{
		return string[0];
	}
	else if (strcmp(string, "SPACE") == 0 || strcmp(string, " ") == 0)
	{
		return sf::Keyboard::Key::Space;
	}
	else if (strcmp(string, "ADD") == 0 || strcmp(string, "+") == 0)
	{
		return sf::Keyboard::Key::Add;
	}
	else if (strcmp(string, "SUBTRACT") == 0 || strcmp(string, "-") == 0)
	{
		return sf::Keyboard::Key::Subtract;
	}
	else if (strcmp(string, "DIVIDE") == 0 || strcmp(string, "/") == 0)
	{
		return sf::Keyboard::Key::Divide;
	}
	else if (strcmp(string, "MULTIPLY") == 0 || strcmp(string, "*") == 0)
	{
		return sf::Keyboard::Key::Multiply;
	}
	else if (strcmp(string, "LEFT") == 0)
	{
		return sf::Keyboard::Key::Left;
	}
	else if (strcmp(string, "RIGHT") == 0)
	{
		return sf::Keyboard::Key::Right;
	}
	else if (strcmp(string, "UP") == 0)
	{
		return sf::Keyboard::Key::Up;
	}
	else if (strcmp(string, "DOWN") == 0)
	{
		return sf::Keyboard::Key::Down;
	}
	else if (strcmp(string, "CTRL") == 0)
	{
		return sf::Keyboard::Key::LControl;
	}
	else if (strcmp(string, "RCTRL") == 0)
	{
		return sf::Keyboard::Key::RControl;
	}
	else if (strcmp(string, "SHIFT") == 0)
	{
		return sf::Keyboard::Key::LShift;
	}
	else if (strcmp(string, "RSHIFT") == 0)
	{
		return sf::Keyboard::Key::RShift;
	}
	else if (strcmp(string, "ESC") == 0)
	{
		return sf::Keyboard::Key::Escape;
	}
	else
	{
		Assert(false, "La tecla %s no existe", string);
		return -1;
	}
}

void CActionManager::LoadActions(const std::string filename)
{
	tinyxml2::XMLDocument document;
	tinyxml2::XMLError error = document.LoadFile((PATH_BASE + filename).c_str());
	if (base::xml::SucceedLoad(error))
	{
		tinyxml2::XMLElement *actions = document.FirstChildElement("actions");
		if (actions)
		{
			for (tinyxml2::XMLElement *action = actions->FirstChildElement(); action != nullptr; action = action->NextSiblingElement())
			{
				if (strcmp(action->Name(), "action") == 0)
				{
					InputAction inputAction = {};
					const char* actionName = action->Attribute("name");
					Assert(actionName != nullptr, "No se ha encontrado nombre de la acción");
					for (tinyxml2::XMLElement *trigger = action->FirstChildElement(); trigger != nullptr; trigger = trigger->NextSiblingElement())
					{
						if (strcmp(trigger->Name(), "trigger") == 0)
						{
							ActionTrigger actionTrigger = {};
							actionTrigger.type = ActionTrigger::GetTriggerTypeFromString(trigger->Attribute("type"));
							switch (actionTrigger.type)
							{
							case ActionTrigger::KEYBOARD:
								actionTrigger.keyboard.keyCode = GetKeyCode(trigger->Attribute("key_code"));
								actionTrigger.keyboard.actionType = ActionTrigger::GetButtonActionFromString(trigger->Attribute("action_type"));
								actionTrigger.keyboard.value = trigger->FloatAttribute("value");
								break;
							case ActionTrigger::MOUSE:
								actionTrigger.mouse.axis = InputDefinitions::GetMouseAxisFromString(trigger->Attribute("mouse_axis"));
								actionTrigger.mouse.geThreshold = ActionTrigger::GetGEThresholdFromString(trigger->Attribute("ge_threshold"));
								actionTrigger.mouse.sensitivity = trigger->FloatAttribute("sensitivity");
								actionTrigger.mouse.invert = trigger->BoolAttribute("invert");
								break;
							case ActionTrigger::MOUSE_BUTTON:
								actionTrigger.mouseButton.button = InputDefinitions::GetMouseButtonFromString(trigger->Attribute("button"));
								actionTrigger.mouseButton.actionType = ActionTrigger::GetButtonActionFromString(trigger->Attribute("action_type"));
								actionTrigger.mouseButton.value = trigger->FloatAttribute("value");
								break;
							case ActionTrigger::GAMEPAD_BUTTON:
								actionTrigger.gamepadButton.button = InputDefinitions::GetGamepadButtonFromString(trigger->Attribute("button"));
								actionTrigger.gamepadButton.actionType = ActionTrigger::GetButtonActionFromString(trigger->Attribute("action_type"));
								actionTrigger.gamepadButton.value = trigger->FloatAttribute("value");
								actionTrigger.gamepadButton.player = trigger->IntAttribute("player");
								break;
							case ActionTrigger::GAMEPAD:
								actionTrigger.gamepad.axis = InputDefinitions::GetGamepadAxisFromString(trigger->Attribute("axis"));
								actionTrigger.gamepad.sensitivity = trigger->FloatAttribute("sensitivity");
								actionTrigger.gamepad.invert = trigger->BoolAttribute("invert");
								actionTrigger.gamepad.player = trigger->IntAttribute("player");
								break;
							}
							inputAction.triggers.push_back(actionTrigger);
						}
					}
					Add(actionName, new InputAction(inputAction));
				}
			}
		}
	}
}

bool CActionManager::IsGamepadConnected(int player) {
	return m_InputManager.IsGamepadConnected(player);
}

bool CActionManager::KeyBecomesPressed(unsigned char keyCode) {
	return  m_InputManager.KeyBecomesPressed(keyCode);
}

bool CActionManager::KeyBecomesReleased(unsigned char keyCode) {
	return  m_InputManager.KeyBecomesReleased(keyCode);
}