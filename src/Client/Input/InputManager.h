#pragma once

#include "InputDefinitions.h"

#define MAX_PLAYERS 1

class CInputManager
{
public:
	CInputManager();
	~CInputManager();
	void Init();

	bool IsKeyPressed(unsigned char keyCode) const;
	bool KeyBecomesPressed(unsigned char keyCode) const;
	bool KeyBecomesReleased(unsigned char keyCode) const;
	bool AnyKeyBecomesReleased() const;
	bool AnyKeyBecomesPressed() const;
	int GetMouseMovementX() const;
	int GetMouseMovementY() const;
	int GetMouseMovementZ() const;
	int GetMouseAxis(InputDefinitions::MouseAxis axis) const;
	int GetMouseX() const;
	int GetMouseY() const;
	bool IsLeftMouseButtonPressed() const;
	bool IsRightMouseButtonPressed() const;
	bool IsMiddleMouseButtonPressed() const;
	bool LeftMouseButtonBecomesPressed() const;
	bool RightMouseButtonBecomesPressed() const;
	bool MiddleMouseButtonBecomesPressed() const;
	bool LeftMouseButtonBecomesReleased() const;
	bool RightMouseButtonBecomesReleased() const;
	bool MiddleMouseButtonBecomesReleased() const;
	bool IsMouseButtonPressed(InputDefinitions::MouseButton button) const;
	bool MouseButtonBecomesPressed(InputDefinitions::MouseButton button) const;
	bool MouseButtonBecomesReleased(InputDefinitions::MouseButton button) const;

	bool IsGamepadConnected(int player);
	bool IsGamepadButtonPressed(InputDefinitions::GamepadButton, int player);
	bool GamepadButtonBecomesPressed(InputDefinitions::GamepadButton, int player);
	bool GamepadButtonBecomesReleased(InputDefinitions::GamepadButton, int player);
	float GetGamepadAxis(InputDefinitions::GamepadAxis axis, int player);

	bool HandleKeyboard(const sf::Event& msg);
	bool HandleMouse(const sf::Event& msg);

	void Update();
	void PreUpdate(bool isWindowActive);
	void PostUpdate();
private:
	InputDefinitions::KeyboardData m_PreviousKeyboardState;
	InputDefinitions::KeyboardData m_KeyboardState;
	int m_MouseMovementX;
	int m_MouseMovementY;
	int m_MouseMovementZ;
	int m_MouseX;
	int m_MouseY;
	bool m_ButtonRight, m_PreviousButtonRight;
	bool m_ButtonLeft, m_PreviousButtonLeft;
	bool m_ButtonMiddle, m_PreviousButtonMiddle;
	// Variables del mando ////////
	bool m_HasGamepad[MAX_PLAYERS];
	// Sticks
	float m_GamepadStickLeftX[MAX_PLAYERS], m_GamepadStickLeftY[MAX_PLAYERS];
	float m_GamepadStickRightX[MAX_PLAYERS], m_GamepadStickRightY[MAX_PLAYERS];
	// Gatillos
	float m_GamepadTL[MAX_PLAYERS];
	float m_GamepadTR[MAX_PLAYERS];
	InputDefinitions::GamepadData m_Gamepad[MAX_PLAYERS];
	InputDefinitions::GamepadData m_PreviousGamepad[MAX_PLAYERS];
};