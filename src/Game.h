#pragma once
//#define GLM_FORCE_LEFT_HANDED
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <sstream>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Camera.h"
#include "ScriptInterpreter.h"
#include "InputManager.h"

class GameData;
class GameWorld;
class GameRenderer;

/*extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}*/

class Game
{
    std::unique_ptr<GameData> gameData;
    std::unique_ptr<GameWorld> gameWorld;
    std::unique_ptr<NativeWindow> window;
    std::unique_ptr<GameRenderer> rendering_system;
    std::unique_ptr<ScriptInterpreter> scriptMachine;
    std::unique_ptr<InputManager> input;
    std::unique_ptr<Camera> camera;

public:
    Game(const char* GamePath);
    ~Game();
    bool paused = false;
    float gameTime;

    void updateFPS(float delta_time, float cpuThreadTime, float gpuThreadTime);
    void run();

    void tick(float delta_time);
    void changePlayer();

    InputManager* getInput() const
    {
        return input.get();
    }

    GameWorld* getWorld() const
    {
        return gameWorld.get();
    }

    GameRenderer* getRenderer() const
    {
        return rendering_system.get();
    }

    GameData* getGameData() const
    {
        return gameData.get();
    }
};
