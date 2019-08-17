#pragma once
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <memory>
#include <sstream>
#include <chrono>
#include "ft2build.h"
#include FT_FREETYPE_H

class Camera;
class ScriptInterpreter;
class InputManager;
class GameData;
class GameWorld;
class GameRenderer;
class NativeWindow;

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
    //std::unique_ptr<NativeWindow> window;
    std::unique_ptr<GameRenderer> rendering_system;
    std::unique_ptr<ScriptInterpreter> scriptMachine;
    std::unique_ptr<InputManager> input;
    std::unique_ptr<Camera> camera;

    std::chrono::steady_clock::time_point current_time;
    
public:
    Game(const char* gamePath);
    ~Game();
    bool paused = false;
    float gameTime;

    void updateFPS(float delta_time, float cpuThreadTime, float gpuThreadTime);
    void run();
    void frame();
    void tick(float delta_time);
    
    void changeLocation();

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
