#pragma once

#include <memory>
#include <sstream>
#include "SDL.h"

#include "RenderingSystem.h"
#include "StateManager.h"
#include "MenuState.h"
#include "InGameState.h"

class Game
{
	SDL_Window *window;

	std::unique_ptr<GameWorld> gameWorld;
	std::unique_ptr<RenderingSystem> rendering_system;

public:
	Game();
	~Game();

	bool paused = false;

	void updateFPS(float delta_time);
	void run();

	GameWorld* getWorld() {
		return gameWorld.get();
	}

	SDL_Window* getWindow() {
		return window;
	}

	RenderingSystem* getRenderer() {
		return rendering_system.get();
	}
};

