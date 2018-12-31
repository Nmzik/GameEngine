#pragma once
#include "SDL.h"
#include <memory>
#include <cstring>

class InputManager
{
	uint8_t PreviousInput[SDL_NUM_SCANCODES];
	uint8_t CurrentInput[SDL_NUM_SCANCODES];

public:
	InputManager();
	~InputManager();

	void Update();

	bool IsKeyTriggered(const SDL_Scancode iKeyCode) const;
	bool IsKeyPressed(const SDL_Scancode iKeyCode) const;
	bool IsKeyReleased(const SDL_Scancode iKeyCode) const;
};
