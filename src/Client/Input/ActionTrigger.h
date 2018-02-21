#pragma once

#include "InputDefinitions.h"

class ActionTrigger
{
public:

	enum TriggerType
	{
		KEYBOARD,
		MOUSE,
		MOUSE_BUTTON,
		GAMEPAD,
		GAMEPAD_BUTTON
	} type;

	enum ButtonActionType
	{
		IsPressed,
		IsReleased,
		BecomesPressed,
		BecomesReleased
	};

	union
	{
		struct
		{
			ButtonActionType actionType;
			unsigned char keyCode;
			float value;
		} keyboard;

		struct MouseButton
		{
			ButtonActionType actionType;
			InputDefinitions::MouseButton button;
			float value;
		} mouseButton;

		struct
		{
			float threshold;
			InputDefinitions::MouseAxis axis;
			bool invert;
			float sensitivity;
			bool geThreshold;
		} mouse;

		struct
		{
			ButtonActionType actionType;
			InputDefinitions::GamepadButton button;
			float value;
			int player;
		} gamepadButton;

		struct
		{
			InputDefinitions::GamepadAxis axis;
			bool invert;
			float sensitivity;
			int player;
		} gamepad;
	};
	static TriggerType GetTriggerTypeFromString(const char* string)
	{
		if (strcmp(string, "KEYBOARD") == 0)
		{
			return TriggerType::KEYBOARD;
		}
		else if (strcmp(string, "MOUSE") == 0)
		{
			return TriggerType::MOUSE;
		}
		else if (strcmp(string, "MOUSE_BUTTON") == 0)
		{
			return TriggerType::MOUSE_BUTTON;
		}
		else if (strcmp(string, "GAMEPAD") == 0)
		{
			return TriggerType::GAMEPAD;
		}
		else if (strcmp(string, "GAMEPAD_BUTTON") == 0)
		{
			return TriggerType::GAMEPAD_BUTTON;
		}
		return TriggerType::KEYBOARD;
	}

	static ButtonActionType GetButtonActionFromString(const char* string)
	{
		if (strcmp(string, "BECOMES_PRESSED") == 0)
		{
			return ButtonActionType::BecomesPressed;
		}
		else if (strcmp(string, "BECOMES_RELEASED") == 0)
		{
			return ButtonActionType::BecomesReleased;
		}
		else if (strcmp(string, "IS_PRESSED") == 0)
		{
			return ButtonActionType::IsPressed;
		}
		else if (strcmp(string, "IS_RELEASED") == 0)
		{
			return ButtonActionType::IsReleased;
		}
		return ButtonActionType::IsPressed;
	}

	static bool GetGEThresholdFromString(const char* string)
	{
		if (strcmp(string, "TRUE") == 0 || strcmp(string, "true") == 0 || strcmp(string, "1") == 0)
		{
			return true;
		}
		else return false;
	}

};