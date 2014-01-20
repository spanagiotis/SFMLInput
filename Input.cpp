#pragma once
#include "Input.hpp"

// Static member declarations
Key Input::keyInfo;
Key Input::previousKeyInfo;
Mouse Input::mouseInfo;
Mouse Input::previousMouseInfo;
bool Input::initialized;
std::list<Key> Input::keyHistory;
int Input::maximumKeyHistoryCount;
sf::RenderWindow* Input::window;

// Initializes input member variables
/*
Member variables cannot be overriden after the inital static initialization.
*/
void Input::Initialize(sf::RenderWindow* w)
{
	if(initialized == true)
		return;

	window = w;
	maximumKeyHistoryCount = 10;
	initialized = true;
}

// Checks the windows messages and calls appropriate message functions.
/*
This should only be called from the main game loop before the rest of the game is updated
for the frame otherwise it may cause in intended behavior. It will poll both the keyboard
and mouse states and then save them into easily accessible structs.
*/
void Input::PollEvents(float elapsedTime)
{
	if(previousMouseInfo.button == mouseInfo.button && previousMouseInfo.state == KeyState::Pressed && mouseInfo.state == KeyState::Pressed)
	{
		mouseInfo.state = KeyState::Held;
	}
	else
	{
		previousMouseInfo = mouseInfo;
	}

	if(previousKeyInfo.decimal == keyInfo.decimal && previousKeyInfo.state == KeyState::Pressed && keyInfo.state == KeyState::Pressed)
	{
		keyInfo.state = KeyState::Held;
		AddToKeyHistory(keyInfo);
	}
	else if (previousKeyInfo.state == KeyState::Released && keyInfo.state == KeyState::Released)
	{
		keyInfo.Reset();
	}
	else
	{
		previousKeyInfo = keyInfo;
	}

	MSG msg;
	if (PeekMessage(&msg,0,0,0, PM_NOREMOVE))
	{
		switch(msg.message)
		{
		case WM_KEYDOWN:
			OnKeyDown(msg.wParam, elapsedTime);
			break;
		case WM_KEYUP:
			OnKeyUp(msg.wParam, elapsedTime);
			break;
		case WM_LBUTTONDOWN:
			OnLeftButtonDown(msg.wParam, elapsedTime);
			break;
		case WM_LBUTTONUP:
			OnLeftButtonUp(msg.wParam, elapsedTime);
			break;
		case WM_RBUTTONDOWN:
			OnRightButtonDown(msg.wParam, elapsedTime);
			break;
		case WM_RBUTTONUP:
			OnRightButtonUp(msg.wParam, elapsedTime);
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	sf::Event event;
	while (window->pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			window->close();
	}


	mouseInfo.mousePosition = sf::Mouse::getPosition((*window));
}

// Returns a copy of keyinfo.
Key Input::GetKeyInfo(void)
{
	return keyInfo;
}

// Returns a copy of mouseInfo.
Mouse Input::GetMouseInfo(void)
{
	return mouseInfo;
}

// Will check to see if the there is a key pressed this frame.
bool Input::IsAKeyPressed(void)
{
	if((previousKeyInfo.decimal != keyInfo.decimal) && keyInfo.state == KeyState::Pressed)
		return true;
	return false;
}

// Will check to see a key was released this frame.
bool Input::IsAKeyReleased(void)
{
	if((previousKeyInfo.decimal == keyInfo.decimal) &&  (keyInfo.state == KeyState::Released && previousKeyInfo.state != KeyState::Released))
		return true;
	return false;
}

// Will check to see if a key was held this frame.
bool Input::IsAKeyHeld(void)
{
	if(keyInfo.state == KeyState::Held && keyInfo.decimal != 0)
		return true;
	return false;
}

// Checks if the specified key is down, this includes pressed & held.
bool Input::IsSpecifiedKeyDown(char key)
{
	if((keyInfo.symbol == key && keyInfo.state == KeyState::Pressed) || (keyInfo.symbol == key && keyInfo.state == KeyState::Held))
		return true;
	return false;
}

// Checks if the specified key is up, this includes released and none.
bool Input::IsSpecifiedKeyUp(char key)
{
	if((keyInfo.symbol == key && keyInfo.state == KeyState::Released) || (keyInfo.symbol != key))
		return true;
	return false;
}

// Will check if the left mouse button was just pressed this frame.
bool Input::IsLeftMouseButtonPressed(void)
{
	if((mouseInfo.button == MouseButton::Left && mouseInfo.state == KeyState::Pressed) &&
		(previousMouseInfo.button != MouseButton::Left && previousMouseInfo.state != KeyState::Pressed))
		return true;
	return false;
}

// Will check if the left mouse button was just released this frame.
bool Input::IsLeftMouseButtonReleased(void)
{
	if((mouseInfo.button == MouseButton::Left && mouseInfo.state == KeyState::Released) &&
		(previousMouseInfo.button == MouseButton::Left && previousMouseInfo.state != KeyState::Released))
		return true;
	return false;
}

// Will check if the left mouse button is held this frame.
bool Input::IsLeftMouseButtonHeld(void)
{
	if(mouseInfo.button == MouseButton::Left && mouseInfo.state == KeyState::Held)
		return true;
	return false;
}

// Will check if the right mouse button was just pressed this frame.
bool Input::IsRightMouseButtonPressed(void)
{
	if((mouseInfo.button == MouseButton::Right && mouseInfo.state == KeyState::Pressed) &&
		(previousMouseInfo.button != MouseButton::Right && previousMouseInfo.state != KeyState::Pressed))
		return true;
	return false;
}

// Will check if the right mouse button was just released this frame.
bool Input::IsRightMouseButtonReleased(void)
{
	if((mouseInfo.button == MouseButton::Right && mouseInfo.state == KeyState::Released) &&
		(previousMouseInfo.button == MouseButton::Right && previousMouseInfo.state != KeyState::Released))
		return true;
	return false;
}

// Will check if the right mouse button is held this frame.
bool Input::IsRightMouseButtonHeld(void)
{
	if(mouseInfo.button == MouseButton::Right && mouseInfo.state == KeyState::Held)
		return true;
	return false;
}

// Returns a copy of the keyHistory list.
/*
The key history list holds all the keys that have had action taken on them, up to the
maximumKeyHistoryCount size. This includes, presses, releases, and held states.
*/
std::list<Key> Input::GetKeyHistory(void)
{
	return keyHistory;
}

// Private methods

// Fires on the WM_KEYDOWN message and sets the keyInfo to a pressed state.
void Input::OnKeyDown(WPARAM key, float elapsedTime)
{
	keyInfo.decimal = key;
	keyInfo.symbol= static_cast<char>(key);
	keyInfo.pressTime = elapsedTime;
	keyInfo.state = KeyState::Pressed;
	AddToKeyHistory(keyInfo);
}

// Fires on the WM_KEYUP message and sets the keyInfo to a released state.
void Input::OnKeyUp(WPARAM key, float elapsedTime)
{
	keyInfo.decimal = key;
	keyInfo.symbol= static_cast<char>(key);
	keyInfo.pressTime = elapsedTime;
	keyInfo.state = KeyState::Released;
	AddToKeyHistory(keyInfo);
}

void Input::OnLeftButtonDown(WPARAM key, float elapsedTime)
{
	mouseInfo.button = MouseButton::Left;
	mouseInfo.pressTime = elapsedTime;
	mouseInfo.state = KeyState::Pressed;
}

void Input::OnLeftButtonUp(WPARAM key, float elapsedTime)
{
	mouseInfo.button = MouseButton::Left;
	mouseInfo.pressTime = elapsedTime;
	mouseInfo.state = KeyState::Released;
}

void Input::OnRightButtonDown(WPARAM key, float elapsedTime)
{
	mouseInfo.button = MouseButton::Right;
	mouseInfo.pressTime = elapsedTime;
	mouseInfo.state = KeyState::Pressed;
}

void Input::OnRightButtonUp(WPARAM key, float elapsedTime)
{
	mouseInfo.button = MouseButton::Right;
	mouseInfo.pressTime = elapsedTime;
	mouseInfo.state = KeyState::Released;
}

void Input::AddToKeyHistory(Key key)
{
	if(keyHistory.size() == maximumKeyHistoryCount)
	{
		keyHistory.pop_back();
	}
	keyHistory.push_front(key);
}