#include "InputManager.h"
#include "Utils\Logger\Logger.h"

#include <SFML\Window.hpp>

CInputManager::CInputManager()
{

}

CInputManager::~CInputManager()
{
	int m_MouseMovementX = 0;
	int m_MouseMovementY = 0;
	int m_MouseMovementZ = 0;
	int m_MouseX = 0;
	int m_MouseY = 0;
	bool m_ButtonRight = false, m_PreviousButtonRight = false;
	bool m_ButtonLeft = false, m_PreviousButtonLeft = false;
	bool m_ButtonMiddle = false, m_PreviousButtonMiddle = false;
	m_HasGamepad[MAX_PLAYERS] = { 0 };
	m_GamepadStickLeftX[MAX_PLAYERS] = { 0.0f }, m_GamepadStickLeftY[MAX_PLAYERS] = { 0.0f };
	m_GamepadStickRightX[MAX_PLAYERS] = { 0.0f }, m_GamepadStickRightY[MAX_PLAYERS] = { 0.0f };
	// Gatillos
	m_GamepadTL[MAX_PLAYERS] = { 0.0f };
	m_GamepadTR[MAX_PLAYERS] = { 0.0f };
}

void CInputManager::Init()
{
	m_ButtonLeft = m_ButtonMiddle = m_ButtonRight = false;
	m_KeyboardState = {};
}

bool CInputManager::IsGamepadConnected(int player)
{
	return m_HasGamepad[player];
}

void CInputManager::Update() {
}

void CInputManager::PreUpdate(bool isWindowActive)
{
	m_PreviousKeyboardState = m_KeyboardState;

	m_PreviousButtonLeft = m_ButtonLeft;
	m_PreviousButtonMiddle = m_ButtonMiddle;
	m_PreviousButtonRight = m_ButtonRight;

	m_MouseMovementX = m_MouseMovementY = m_MouseMovementZ = 0;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_PreviousGamepad[i] = m_Gamepad[i];
		m_GamepadStickLeftX[i] = m_GamepadStickLeftY[i] = m_GamepadStickRightX[i] = m_GamepadStickRightY[i] = m_GamepadTL[i] = m_GamepadTR[i] = 0.0f;
	}

	if (!isWindowActive)
	{
		m_KeyboardState = {};
		m_ButtonLeft = m_ButtonMiddle = m_ButtonRight = false;
	}
}

void CInputManager::PostUpdate()
{
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		if (sf::Joystick::isConnected(i))
		{
			m_HasGamepad[i] = true;
			m_Gamepad[i].SetButtons(i);

			m_GamepadStickLeftX[i] = sf::Joystick::getAxisPosition(i, sf::Joystick::Axis::X);
			m_GamepadStickLeftY[i] = sf::Joystick::getAxisPosition(i, sf::Joystick::Axis::Y);
			m_GamepadStickRightX[i] = sf::Joystick::getAxisPosition(i, sf::Joystick::Axis::U);
			m_GamepadStickRightY[i] = sf::Joystick::getAxisPosition(i, sf::Joystick::Axis::R);

			m_GamepadTL[i] = sf::Joystick::getAxisPosition(i, sf::Joystick::Axis::Z);
			m_GamepadTR[i] = -sf::Joystick::getAxisPosition(i, sf::Joystick::Axis::Z);
		}
		else {
			m_HasGamepad[i] = false;
		}
	}

}

bool CInputManager::HandleKeyboard(const sf::Event& msg)
{
	unsigned char lKeyCode = (unsigned char)msg.key.code;

	if (msg.type == sf::Event::EventType::KeyPressed)
	{
		m_KeyboardState.SetKey(lKeyCode, true);
		return true;
	}
	else if (msg.type == sf::Event::EventType::KeyReleased) {
		m_KeyboardState.SetKey(lKeyCode, false);
		return true;
	}
	else if (msg.type == sf::Event::EventType::TextEntered) {
		// TODO
	}

	return false;
}

bool CInputManager::HandleMouse(const sf::Event& msg)
{
	switch (msg.type)
	{
	case sf::Event::EventType::MouseButtonPressed:
		if (msg.mouseButton.button == sf::Mouse::Button::Left)
			m_ButtonLeft = true;
		if (msg.mouseButton.button == sf::Mouse::Button::Right)
			m_ButtonRight = true;
		if (msg.mouseButton.button == sf::Mouse::Button::Middle)
			m_ButtonMiddle = true;
		return true;
	case sf::Event::EventType::MouseButtonReleased:
		if (msg.mouseButton.button == sf::Mouse::Button::Left)
			m_ButtonLeft = false;
		if (msg.mouseButton.button == sf::Mouse::Button::Right)
			m_ButtonRight = false;
		if (msg.mouseButton.button == sf::Mouse::Button::Middle)
			m_ButtonMiddle = false;
		return true;
	case sf::Event::EventType::MouseMoved:
		m_MouseMovementX = msg.mouseMove.x;
		m_MouseMovementY = msg.mouseMove.y;
		m_MouseX = sf::Mouse::getPosition().x;
		m_MouseY = sf::Mouse::getPosition().y;
		return true;
	case sf::Event::EventType::MouseWheelScrolled:
		m_MouseMovementZ = msg.mouseWheelScroll.delta;
		return true;
	}

	return false;
}

bool CInputManager::IsLeftMouseButtonPressed() const
{
	return m_ButtonLeft;
}

bool CInputManager::IsRightMouseButtonPressed() const
{
	return m_ButtonRight;
}

bool CInputManager::IsMiddleMouseButtonPressed() const
{
	return m_ButtonMiddle;
}

bool CInputManager::IsKeyPressed(unsigned char keyCode) const
{
	return m_KeyboardState.raw[keyCode];
}

bool CInputManager::KeyBecomesPressed(unsigned char keyCode) const
{
	return (!m_PreviousKeyboardState.raw[keyCode] && m_KeyboardState.raw[keyCode]);
}

bool CInputManager::KeyBecomesReleased(unsigned char keyCode) const
{
	return (m_PreviousKeyboardState.raw[keyCode] && !m_KeyboardState.raw[keyCode]);
}

bool CInputManager::AnyKeyBecomesReleased() const {
	return  (!m_KeyboardState.anyKeyDown && m_PreviousKeyboardState.anyKeyDown);
}

bool CInputManager::AnyKeyBecomesPressed() const {
	return (m_KeyboardState.anyKeyDown && !m_PreviousKeyboardState.anyKeyDown);
}

int CInputManager::GetMouseMovementX() const
{
	return m_MouseMovementX;
}

int CInputManager::GetMouseMovementY() const
{
	return m_MouseMovementY;
}

int CInputManager::GetMouseMovementZ() const
{
	return m_MouseMovementZ;
}

int CInputManager::GetMouseAxis(InputDefinitions::MouseAxis axis) const
{
	switch (axis)
	{
	case InputDefinitions::DX:
		return GetMouseMovementX();
	case InputDefinitions::DY:
		return GetMouseMovementY();
	case InputDefinitions::DZ:
		return GetMouseMovementZ();
	default:
		return 0;
	}
}

int CInputManager::GetMouseX() const
{
	return m_MouseX;
}

int CInputManager::GetMouseY() const
{
	return m_MouseY;
}

bool CInputManager::LeftMouseButtonBecomesPressed() const
{
	return (!m_PreviousButtonLeft && m_ButtonLeft);
}

bool CInputManager::RightMouseButtonBecomesPressed() const
{
	return (!m_PreviousButtonRight && m_ButtonRight);
}

bool CInputManager::MiddleMouseButtonBecomesPressed() const
{
	return (!m_PreviousButtonMiddle && m_ButtonMiddle);
}

bool CInputManager::LeftMouseButtonBecomesReleased() const
{
	return (m_PreviousButtonLeft && !m_ButtonLeft);
}

bool CInputManager::RightMouseButtonBecomesReleased() const
{
	return (m_PreviousButtonRight && !m_ButtonRight);
}

bool CInputManager::MiddleMouseButtonBecomesReleased() const
{
	return (m_PreviousButtonMiddle && !m_ButtonMiddle);
}

bool CInputManager::IsMouseButtonPressed(InputDefinitions::MouseButton button) const
{
	switch (button)
	{
	case InputDefinitions::MouseButton::LEFT:
		return IsLeftMouseButtonPressed();
	case InputDefinitions::MouseButton::RIGHT:
		return IsRightMouseButtonPressed();
	case InputDefinitions::MouseButton::MIDDLE:
		return IsMiddleMouseButtonPressed();
	default:
		return false;
	}
}

bool CInputManager::MouseButtonBecomesPressed(InputDefinitions::MouseButton button) const
{
	switch (button)
	{
	case InputDefinitions::MouseButton::LEFT:
		return LeftMouseButtonBecomesPressed();
	case InputDefinitions::MouseButton::RIGHT:
		return RightMouseButtonBecomesPressed();
	case InputDefinitions::MouseButton::MIDDLE:
		return MiddleMouseButtonBecomesPressed();
	default:
		return false;
	}
}

bool CInputManager::MouseButtonBecomesReleased(InputDefinitions::MouseButton button) const
{
	switch (button)
	{
	case InputDefinitions::MouseButton::LEFT:
		return LeftMouseButtonBecomesReleased();
	case InputDefinitions::MouseButton::RIGHT:
		return RightMouseButtonBecomesReleased();
	case InputDefinitions::MouseButton::MIDDLE:
		return MiddleMouseButtonBecomesReleased();
	default:
		return false;
	}
}

bool CInputManager::IsGamepadButtonPressed(InputDefinitions::GamepadButton button, int player)
{
	if (!m_HasGamepad[player]) return false;
	return m_Gamepad[player].GetButton(button);
}

bool CInputManager::GamepadButtonBecomesPressed(InputDefinitions::GamepadButton button, int player)
{
	if (!m_HasGamepad[player]) return false;
	bool prev = m_PreviousGamepad[player].GetButton(button);
	bool act = m_Gamepad[player].GetButton(button);
	return (!prev && act);
}

bool CInputManager::GamepadButtonBecomesReleased(InputDefinitions::GamepadButton button, int player)
{
	if (!m_HasGamepad[player]) return false;
	return (m_PreviousGamepad[player].GetButton(button) && !m_Gamepad[player].GetButton(button));
}

float CInputManager::GetGamepadAxis(InputDefinitions::GamepadAxis axis, int player)
{
	if (!m_HasGamepad[player]) return 0.0f;
	switch (axis)
	{
	case InputDefinitions::GamepadAxis::LEFT_THUMB_X:
		return m_GamepadStickLeftX[player];
	case InputDefinitions::GamepadAxis::LEFT_THUMB_Y:
		return m_GamepadStickLeftY[player];
	case InputDefinitions::GamepadAxis::RIGHT_THUMB_X:
		return m_GamepadStickRightX[player];
	case InputDefinitions::GamepadAxis::RIGHT_THUMB_Y:
		return m_GamepadStickRightY[player];
	case InputDefinitions::GamepadAxis::LEFT_TRIGGER:
		return m_GamepadTL[player];
	case InputDefinitions::GamepadAxis::RIGHT_TRIGGER:
		return m_GamepadTR[player];
	default:
		return 0.0f;;
	}
}