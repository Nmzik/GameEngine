#pragma once
#include "SDL.h"
#include "iostream"
#include "RenderingSystem.h"

class InputSystem
{
	RenderingSystem* _RenderingSystem;
public:
	InputSystem(RenderingSystem* parent);
	~InputSystem();

	void update(SDL_Event& event, float delta_time);
};

