#pragma once
#include <memory>
#include <sstream>
#include "SDL.h"

#include "RenderingSystem.h"
#include "InputSystem.h"

class Game
{
	SDL_Window *window;

	std::unique_ptr<GameWorld> gameWorld;
	std::unique_ptr<RenderingSystem> rendering_system;
	std::unique_ptr<InputSystem> input_system;
public:
	Game();
	~Game();

	void updateFPS(float delta_time);
	void run();
};

