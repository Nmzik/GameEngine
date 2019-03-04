#pragma once
//#define GLM_FORCE_LEFT_HANDED
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <sstream>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Camera.h"

#ifdef _WIN32
#include "windows/InputManager.h"
#include "windows/NativeWindow.h"
#include "windows/RenderingSystem.h"
#else

#endif

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
    std::unique_ptr<NativeWindow> window;

    std::unique_ptr<GameData> gameData;
    std::unique_ptr<GameWorld> gameWorld;
    std::unique_ptr<RenderingSystem> rendering_system;
    std::unique_ptr<InputManager> input;
    std::unique_ptr<Camera> camera;

public:
    Game(const char* GamePath);
    ~Game();
    bool paused = false;

    void updateFPS(float delta_time, float cpuThreadTime, float gpuThreadTime);
    void run();

    void tick(float delta_time);
    void changePlayer();

    InputManager* getInput()
    {
        return input.get();
    }

    GameWorld* getWorld()
    {
        return gameWorld.get();
    }

    RenderingSystem* getRenderer()
    {
        return rendering_system.get();
    }
};
