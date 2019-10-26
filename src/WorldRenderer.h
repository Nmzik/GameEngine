#pragma once

#include "BaseRenderer.h"
#include "Geometry.h"
#include "NativeWindow.h"
#include "Texture.h"
#include "TextureManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

class GameWorld;
class Camera;
class Shader;
//
class YdrLoader;
class CBuilding;
class CPed;
class CVehicle;

class WorldRenderer
{
    std::unique_ptr<BaseRenderer> renderer;
    int numDrawCalls;

public:
    WorldRenderer(NativeWindow* window);
    ~WorldRenderer();

    void renderWorld(GameWorld* world, Camera* curCamera);

    BaseRenderer* getBaseRenderer() const
    {
        return renderer.get();
	}

    int getNumDrawCalls() const
    {
        return numDrawCalls;
    }
    void renderDrawable(YdrLoader* drawable);
    void renderBuilding(CBuilding* building);
    void renderPed(CPed* ped);
    void renderVehicle(CVehicle* vehicle);
};
