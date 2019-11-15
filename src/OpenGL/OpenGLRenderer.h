#pragma once
#include "SDL.h"
#include "opengl.h"
#include "..//NativeWindow.h"
#include "../BaseRenderer.h"

class NativeWindow;

class GameWorld;
class Camera;
class Shader;
//
class YdrLoader;
class CBuilding;
class CPed;
class CVehicle;

//#define GL_DEBUG

#ifdef GL_DEBUG
#define GL_CHECK(call)                              \
    call;                                           \
    {                                               \
        GLenum err = glGetError();                  \
        if (err != 0)                               \
        {                                           \
            printf("GL error 0x%x %u\n", err, err); \
            DebugBreak();                           \
        }                                           \
    }
#else
#define GL_CHECK(call) call
#endif

struct GlobalSceneData
{
    glm::mat4 projection;
    glm::mat4 view;
};

class OpenGLRenderer : public BaseRenderer
{
    NativeWindow* nativeWindow;
	//
    GlobalSceneData sceneData;
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

	std::vector<VertexLayout> layouts;
public:
    OpenGLRenderer(NativeWindow* window);
    ~OpenGLRenderer();

    void beginFrame();

    void endFrame();

    virtual void presentFrame() override;

    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;
    virtual uint32_t getLayoutHandle(VertexType type);

    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    virtual void removeTexture(TextureHandle handle) override;

    virtual void updateGlobalSceneBuffer(glm::mat4& Projection, glm::mat4& View) override;
    virtual void updatePerModelData(glm::mat4& mat) override;

	virtual void renderGeom(Geometry& geom) override;
};