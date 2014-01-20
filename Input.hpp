#pragma once
#include <iostream>
#include <list>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#define NULL_TIME -1.0f;

namespace KeyState
{
	enum KeyState
	{
		Pressed = 0,
		Released,
		Held,
		None
	};
}

namespace MouseButton
{
	enum MouseButton
	{
		Left = 0,
		Right,
		Middle,
		None
	};
}

struct Key
{
	Key() { Reset(); }
	Key(char s, int d, float pt, KeyState::KeyState st) : symbol(s), decimal(d), pressTime(pt), state(st) { }
	char symbol;
	int decimal;
	float pressTime;
	KeyState::KeyState state;
	bool SameKey(char rhsKey) { if(symbol == rhsKey) return true; return false; }
	void Reset() { symbol = char(0); decimal = 0; pressTime = NULL_TIME; state = KeyState::None; } 
};

struct Mouse
{
	Mouse() { Reset(); }
	Mouse(MouseButton::MouseButton b, float pt) : button(b), pressTime(pt) { }
	MouseButton::MouseButton button;
	float pressTime;
	KeyState::KeyState state;
	sf::Vector2i mousePosition;
	void Reset() { button = MouseButton::None; pressTime = NULL_TIME; state=KeyState::None; }
};

class Input
{

public:
	~Input(void) {};
	static void Initialize(sf::RenderWindow* window);
	static void PollEvents(float elapsedTime);
	static Key GetKeyInfo(void);
	static Mouse GetMouseInfo(void);
	static bool IsAKeyPressed(void);
	static bool IsAKeyReleased(void);
	static bool IsAKeyHeld(void);
	static bool IsSpecifiedKeyDown(char key);
	static bool IsSpecifiedKeyUp(char key); 
	static bool IsLeftMouseButtonPressed(void);
	static bool IsLeftMouseButtonReleased(void);
	static bool IsLeftMouseButtonHeld(void);
	static bool IsRightMouseButtonPressed(void);
	static bool IsRightMouseButtonReleased(void);
	static bool IsRightMouseButtonHeld(void);
	static std::list<Key> GetKeyHistory(void);

private:
	Input(void) { };
	static void OnKeyDown(WPARAM key, float elapsedTime);
	static void OnKeyUp(WPARAM key, float elapsedTime);
	static void OnLeftButtonDown(WPARAM key, float elapsedTime);
	static void OnLeftButtonUp(WPARAM key, float elapsedTime);
	static void OnRightButtonDown(WPARAM key, float elapsedTime);
	static void OnRightButtonUp(WPARAM key, float elapsedTime);
	static void AddToKeyHistory(Key key);
	static Key keyInfo;
	static Key previousKeyInfo;
	static Mouse mouseInfo;
	static Mouse previousMouseInfo;
	static bool initialized;
	static std::list<Key> keyHistory;
	static int maximumKeyHistoryCount;
	static sf::RenderWindow* window;
};