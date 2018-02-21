#pragma once

#include "Utils\Logger\Logger.h"
#include <SFML\Window.hpp>

#define BUTTON_A sf::Joystick::X // A button on controller
#define BUTTON_X sf::Joystick::Z // X button on controller
#define BUTTON_B sf::Joystick::Y // B button on controller
#define BUTTON_Y sf::Joystick::R // Y button on controller
#define BUTTON_LB sf::Joystick::U // Left bumper on controller
#define BUTTON_RB sf::Joystick::V // Right bumper on controller
#define AXIS_LEFT_X sf::Joystick::Axis::X //  Left stick x value
#define AXIS_LEFT_Y sf::Joystick::Axis::Y // Left stick y value
#define AXIS_RIGHT_X sf::Joystick::Axis::U // Right stick x value
#define AXIS_RIGHT_Y sf::Joystick::Axis::R // Right stick y value
#define TRIGGERS sf::Joystick::Axis::Z // Positive Z = LT, and Negative Z = RT
#define BUTTON_BACK sf::Joystick::PovX // Back button on controller
#define BUTTON_START sf::Joystick::PovY // Start button on controller
#define DPAD_X sf::Joystick::Axis::PovY // Dpad left and right
#define DPAD_Y sf::Joystick::Axis::PovX // Dpad Up and down


namespace InputDefinitions
{
	

	struct KeyboardData
	{
		bool raw[256];
		bool escape, space;
		bool numpad[10];
		bool fkey[24];
		bool left, right, up, down;
		bool anyKeyDown;
		unsigned char lastChar;


		void SetLastChar(unsigned char key) {
			lastChar = key;
		}

		void SetKey(unsigned char key, bool state)
		{

			raw[key] = state;
			lastChar = key;

			if (key >= sf::Keyboard::Key::Num0 || key <= sf::Keyboard::Key::Num9)
			{
				numpad[key - sf::Keyboard::Key::Num0] = state;
			}
			else if (key >= sf::Keyboard::Key::F1 || key <= sf::Keyboard::Key::F15)
			{
				fkey[key - sf::Keyboard::Key::F1] = state;
			}
			else
			{
				switch (key)
				{
				case sf::Keyboard::Key::Escape:
					escape = state;
					break;
				case sf::Keyboard::Key::Space:
					space = state;
					break;
				case sf::Keyboard::Key::Left:
					left = state;
					break;
				case sf::Keyboard::Key::Up:
					up = state;
					break;
				case sf::Keyboard::Key::Right:
					right = state;
					break;
				case sf::Keyboard::Key::Down:
					down = state;
					break;
				}
			}
		};
	};

	enum MouseButton
	{
		LEFT, MIDDLE, RIGHT
	};
	enum MouseAxis
	{
		MOUSE_X, MOUSE_Y, DX, DY, DZ
	};

	enum GamepadButton {
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,
		LB = 0x0100,
		RB = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
	};

	enum GamepadAxis
	{
		LEFT_THUMB_X, LEFT_THUMB_Y, RIGHT_THUMB_X, RIGHT_THUMB_Y, LEFT_TRIGGER, RIGHT_TRIGGER
	};

	struct GamepadData
	{
		// Botones de la derecha
		bool m_GamepadA;
		bool m_GamepadB;
		bool m_GamepadX;
		bool m_GamepadY;
		// Botones traseros
		bool m_GamepadLB;
		bool m_GamepadRB;
		// Botones de los sticks
		bool m_GamepadPL;
		bool m_GamepadPR;
		// Cruceta
		bool m_GamepadLeft;
		bool m_GamepadUp;
		bool m_GamepadRight;
		bool m_GamepadDown;
		// Botones de funcion
		bool m_GamepadStart;
		bool m_GamepadBack;

		void SetButtons(int joystick)
		{
			m_GamepadA = sf::Joystick::isButtonPressed(joystick, BUTTON_A);
			m_GamepadB = sf::Joystick::isButtonPressed(joystick, BUTTON_B);
			m_GamepadX = sf::Joystick::isButtonPressed(joystick, BUTTON_X);
			m_GamepadY = sf::Joystick::isButtonPressed(joystick, BUTTON_Y);

			m_GamepadLB = sf::Joystick::isButtonPressed(joystick, BUTTON_LB);
			m_GamepadRB = sf::Joystick::isButtonPressed(joystick, BUTTON_RB);

			//m_GamepadPL = sf::Joystick::isButtonPressed(joystick, sf::Joystick::);
			//m_GamepadPR = sf::Joystick::isButtonPressed(joystick, GamepadButton::RIGHT_THUMB);

			/*m_GamepadLeft = sf::Joystick::isButtonPressed(joystick, GamepadButton::DPAD);
			m_GamepadUp = sf::Joystick::isButtonPressed(joystick, GamepadButton::DPAD_UP);
			m_GamepadRight = sf::Joystick::isButtonPressed(joystick, GamepadButton::DPAD_RIGHT);
			m_GamepadDown = sf::Joystick::isButtonPressed(joystick, GamepadButton::DPAD_DOWN);*/

			m_GamepadStart = sf::Joystick::isButtonPressed(joystick, BUTTON_START);
			m_GamepadBack = sf::Joystick::isButtonPressed(joystick, BUTTON_BACK);

		}

		bool GetButton(GamepadButton button)
		{
			switch (button)
			{
			case GamepadButton::A:
				return m_GamepadA;
			case GamepadButton::B:
				return m_GamepadB;
			case GamepadButton::X:
				return m_GamepadX;
			case GamepadButton::Y:
				return m_GamepadY;
			case GamepadButton::LB:
				return m_GamepadLB;
			case GamepadButton::RB:
				return m_GamepadRB;
			case GamepadButton::LEFT_THUMB:
				return m_GamepadPL;
			case GamepadButton::RIGHT_THUMB:
				return m_GamepadPR;
			case GamepadButton::DPAD_DOWN:
				return m_GamepadDown;
			case GamepadButton::DPAD_LEFT:
				return m_GamepadLeft;
			case GamepadButton::DPAD_RIGHT:
				return m_GamepadRight;
			case GamepadButton::DPAD_UP:
				return m_GamepadUp;
			case GamepadButton::START:
				return m_GamepadStart;
			case GamepadButton::BACK:
				return m_GamepadBack;
			default:
				return false;
			}
		}
	};

	inline MouseButton GetMouseButtonFromString(const char* str, MouseButton defaultValue = (MouseButton)-1)
	{
		if (str == nullptr)
			return defaultValue;
		else if (strcmp(str, "LEFT") == 0)
			return LEFT;
		else if (strcmp(str, "MIDDLE") == 0)
			return MIDDLE;
		else if (strcmp(str, "RIGHT") == 0)
			return RIGHT;
		else
			return (MouseButton)-1;
	}

	inline MouseAxis GetMouseAxisFromString(const char* str, MouseAxis defaultValue = (MouseAxis)-1)
	{
		if (str == nullptr)
			return defaultValue;
		else if (strcmp(str, "MOUSE_X") == 0)
			return MOUSE_X;
		else if (strcmp(str, "MOUSE_Y") == 0)
			return MOUSE_Y;
		else if (strcmp(str, "DX") == 0)
			return DX;
		else if (strcmp(str, "DY") == 0)
			return DY;
		else if (strcmp(str, "DZ") == 0)
			return DZ;
		else
			return (MouseAxis)-1;
	}

	inline GamepadButton GetGamepadButtonFromString(const char* str, GamepadButton defaultValue = (GamepadButton)-1)
	{
		if (strcmp(str, "A") == 0)
		{
			return GamepadButton::A;
		}
		else if (strcmp(str, "B") == 0)
		{
			return GamepadButton::B;
		}
		else if (strcmp(str, "X") == 0)
		{
			return GamepadButton::X;
		}
		else if (strcmp(str, "Y") == 0)
		{
			return GamepadButton::Y;
		}
		else if (strcmp(str, "LB") == 0)
		{
			return GamepadButton::LB;
		}
		else if (strcmp(str, "RB") == 0)
		{
			return GamepadButton::RB;
		}
		else if (strcmp(str, "UP") == 0)
		{
			return GamepadButton::DPAD_UP;
		}
		else if (strcmp(str, "DOWN") == 0)
		{
			return GamepadButton::DPAD_DOWN;
		}
		else if (strcmp(str, "LEFT") == 0)
		{
			return GamepadButton::DPAD_LEFT;
		}
		else if (strcmp(str, "RIGHT") == 0)
		{
			return GamepadButton::DPAD_RIGHT;
		}
		else if (strcmp(str, "PL") == 0)
		{
			return GamepadButton::LEFT_THUMB;
		}
		else if (strcmp(str, "PR") == 0)
		{
			return GamepadButton::RIGHT_THUMB;
		}
		else if (strcmp(str, "BACK") == 0)
		{
			return GamepadButton::BACK;
		}
		else if (strcmp(str, "START") == 0)
		{
			return GamepadButton::START;
		}
		return (GamepadButton)-1;
	}

	inline GamepadAxis GetGamepadAxisFromString(const char* str, GamepadAxis defaultValue = (GamepadAxis)-1)
	{
		if (strcmp(str, "LEFTX") == 0)
		{
			return GamepadAxis::LEFT_THUMB_X;
		}
		else if (strcmp(str, "LEFTY") == 0)
		{
			return GamepadAxis::LEFT_THUMB_Y;
		}
		else if (strcmp(str, "RIGHTX") == 0)
		{
			return GamepadAxis::RIGHT_THUMB_X;
		}
		else if (strcmp(str, "RIGHTY") == 0)
		{
			return GamepadAxis::RIGHT_THUMB_Y;
		}
		else if (strcmp(str, "TR") == 0)
		{
			return GamepadAxis::RIGHT_TRIGGER;
		}
		else if (strcmp(str, "TL") == 0)
		{
			return GamepadAxis::LEFT_TRIGGER;
		}
		return (GamepadAxis)-1;
	}
}