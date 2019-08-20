#pragma once

#include "Geometry.h"
#include "Texture.h"

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
    int numDrawCalls;

public:
    GameRenderer(/*NativeWindow* window*/);
    virtual ~GameRenderer();

    bool renderDebugWorld;

    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) = 0;
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) = 0;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) = 0;

    virtual void removeTexture(TextureHandle handle) = 0;
    virtual void removeVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void removeIndexbuffer(IndexBufferHandle handle) = 0;
    /*void beginFrame();
    void endFrame();
    void presentFrame();

    void renderDrawable(YdrLoader* drawable);
    void renderBuilding(CBuilding* building);
    void renderPed(CPed* ped);
    void renderVehicle(CVehicle* vehicle);*/
    //
    virtual void renderWorld(GameWorld* world, Camera* curCamera) = 0;

    int getNumDrawCalls() const
    {
        return numDrawCalls;
    }

    //
};
