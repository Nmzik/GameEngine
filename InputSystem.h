#pragma once
#include "SDL.h"
#include "iostream"
#include "RenderingSystem.h"

class InputSystem
{
	RenderingSystem* _RenderingSystem;
	GameWorld* _world;
public:
	InputSystem(RenderingSystem* parent, GameWorld* world);
	~InputSystem();

	void update(SDL_Event& event, float delta_time);
};

