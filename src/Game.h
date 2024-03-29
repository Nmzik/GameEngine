#pragma once
//#define GLM_FORCE_LEFT_HANDED

#include <chrono>
#include <memory>

#include "BaseRenderer.h"
#include "WorldRenderer.h"

class Camera;
class ScriptInterpreter;
class InputManager;
class GameData;
class GameWorld;
class NativeWindow;
class ResourceManager;

/*extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}*/

class Game
{
    std::unique_ptr<GameData> gameData;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<GameWorld> gameWorld;
    std::unique_ptr<WorldRenderer> rendering_system;
    //std::unique_ptr<btIDebugDraw> debugRenderer;
    std::unique_ptr<ScriptInterpreter> scriptMachine;
    std::unique_ptr<InputManager> input;
    std::unique_ptr<Camera> camera;

    std::chrono::steady_clock::time_point current_time;
    std::unique_ptr<NativeWindow> window;

public:
    Game(const char* gamePath, const char* keysPath);
    ~Game();

    void postLoad();
    void initializeCamera(float width, float height);
    bool paused = false;
    float gameTime;

    void updateFPS(float delta_time, float cpuThreadTime, float gpuThreadTime);
    void run();
    void frame();
    void tick(float delta_time);

    void changeLocation();

	NativeWindow* getNativeWindow() const 
	{
		return window.get();
	}

    InputManager* getInput() const
    {
        return input.get();
    }

    GameWorld* getWorld() const
    {
        return gameWorld.get();
    }

    BaseRenderer* getRenderer() const
    {
        return rendering_system.get()->getBaseRenderer();
    }

    GameData* getGameData() const
    {
        return gameData.get();
    }

    ResourceManager* getResourceManager() const
    {
        return resourceManager.get();
    }
};
