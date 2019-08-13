#pragma once

class GameWorld;
class Camera;
class Shader;
//
class YdrLoader;
class CBuilding;
class CPed;
class CVehicle;

class GameRenderer
{
    //NativeWindow* nativeWindow;
    
    int NumDrawCalls;
public:
    GameRenderer(/*NativeWindow* window*/);
    ~GameRenderer();

    bool RenderDebugWorld;

    void beginFrame();
    void endFrame();
    void presentFrame();

    void renderDrawable(YdrLoader* drawable);
    void renderBuilding(CBuilding* building);
    void renderPed(CPed* ped);
    void renderVehicle(CVehicle* vehicle);
    //
    void renderWorld(GameWorld* world, Camera* curCamera);

    int getNumDrawCalls() const
    {
        return NumDrawCalls;
    }

    //
};
