#pragma once
//#define GLM_FORCE_LEFT_HANDED
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "SDL.h"
#include "ft2build.h"
#include <sstream>
#include FT_FREETYPE_H

#include "InputManager.h"

#include "FreeListAllocator.h"
#include <LinearMath/btAlignedAllocator.h>

class GameData;
class GameWorld;
class RenderingSystem;

/*extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}*/

class Game
{
	SDL_Window* window;

	std::unique_ptr<GameData> gameData;
	std::unique_ptr<GameWorld> gameWorld;
	std::unique_ptr<RenderingSystem> rendering_system;
	std::unique_ptr<InputManager> input;

	public:
	Game(const char* GamePath);
	~Game();
	bool paused = false;

	void updateFPS(float delta_time, float cpuThreadTime, float gpuThreadTime);
	void run();

	void tick(float delta_time);
	void ChangePlayer();

	InputManager* getInput()
	{
		return input.get();
	}

	GameWorld* getWorld()
	{
		return gameWorld.get();
	}

	SDL_Window* getWindow()
	{
		return window;
	}

	RenderingSystem* getRenderer()
	{
		return rendering_system.get();
	}
};
