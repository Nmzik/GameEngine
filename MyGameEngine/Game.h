#pragma once

#include <memory>
#include <sstream>
#include "SDL.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "RenderingSystem.h"
#include "GameData.h"
#include "StateManager.h"
#include "MenuState.h"
#include "InGameState.h"
#include "InputManager.h"

class Game
{
	SDL_Window *window;

	std::unique_ptr<GameWorld> gameWorld;
	std::unique_ptr<RenderingSystem> rendering_system;
	InputManager* input;

public:
	Game();
	~Game();
	bool paused = false;

	void updateFPS(float delta_time);
	void run();

	void tick(float delta_time);

	InputManager* getInput() {
		return input;
	}

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
