#pragma once
#include "..//NativeWindow.h"
#include "../GameRenderer.h"
#include "SDL.h"
#include "opengl.h"

class NativeWindow;

class GameWorld;
class Camera;
class Shader;
//
class YdrLoader;
class CBuilding;
class CPed;
class CVehicle;

class OpenGLRenderer : public GameRenderer
{
    NativeWindow* nativeWindow;
    //
    GLuint uboGlobal;
    GLuint uboModel;

    Shader* mainShader;
    GLuint VAO;
    //
    GLuint vertexArrays[gpuBufferSize] = {0};
    GLuint vertexBuffers[gpuBufferSize] = {0};
    GLuint indexBuffers[gpuBufferSize] = {0};
    GLuint textures[gpuBufferSize] = {0};

public:
    OpenGLRenderer(NativeWindow* window);
    ~OpenGLRenderer();

    void beginFrame();

    void endFrame();

    void presentFrame();

    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;

    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    virtual void removeTexture(TextureHandle handle) override;

    void renderDrawable(YdrLoader* drawable);
    void renderBuilding(CBuilding* building);
    void renderPed(CPed* ped);
    void renderVehicle(CVehicle* vehicle);
    //
    void renderWorld(GameWorld* world, Camera* curCamera) override;
};