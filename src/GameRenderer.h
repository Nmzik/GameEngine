#pragma once
#include "Windows/Win32Window.h"

class GameWorld;
class Camera;
class Shader;

class GameRenderer
{
    NativeWindow* nativeWindow;
    GLuint defaultTexture;
    int NumDrawCalls;

    //
    GLuint uboGlobal;
    GLuint uboModel;

    Shader* mainShader;

public:
    GameRenderer(NativeWindow* window);
    ~GameRenderer();

    void beginFrame();
    void endFrame();
    void presentFrame();

    void RenderWorld(GameWorld* world, Camera* curCamera);

    int getNumDrawCalls() const
    {
        return NumDrawCalls;
    }

    //
};
/*#include <iostream>
#include <memory>
#include <random>

#include "NativeWindow.h"
#include "SDL.h"
#include "opengl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "Camera.h"
#include "Shader.h"
#include "Skybox.h"

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

class GameWorld;

class GameRenderer
{
private:
    GLuint uboGlobal;
    GLuint uboModel;

    std::unique_ptr<Shader> SkyboxShader;

    std::unique_ptr<Shader> gbuffer;
    std::unique_ptr<Shader> Transparentgbuffer;

    std::unique_ptr<Shader> shaderSSAO;
    std::unique_ptr<Shader> shaderSSAOBlur;
    std::unique_ptr<Shader> gbufferLighting;
    std::unique_ptr<Shader> DepthTexture;
    std::unique_ptr<Shader> hdrShader;
    std::unique_ptr<Shader> debugDepthQuad;
    //std::unique_ptr<Camera> camera;
    Skybox* skybox;

    unsigned int quadVAO;
    unsigned int quadVBO;
    int type = 0;

    int ScreenResWidth;
    int ScreenResHeight;
    int ShadowWidth;
    int ShadowHeight;

    unsigned int depthMapFBO;
    unsigned int depthMap;
    unsigned int ssaoFBO, ssaoBlurFBO;
    unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
    std::vector<glm::vec3> ssaoKernel;
    unsigned int noiseTexture;
    ///
    unsigned int hdrFBO;
    unsigned int colorBuffer;
    //	G-BUFFER
    unsigned int gBuffer;
    unsigned int gNormal, gAlbedoSpec, gDepthMap;

    GLuint m_nQueryIDDrawTime;
    GLuint DefaultTexture;
    void skyboxPass();

    SDL_Window* window;
    SDL_GLContext glcontext;

public:
    GameRenderer(NativeWindow* window_);
    ~GameRenderer();

    bool ShowTexture = false;
    bool gpuTimer = false;
    bool mWaiting = false;
    GLuint gpuTime = 0;
    uint32_t DrawCalls;
    glm::vec3 lightPos;
    bool RenderDebugWorld = false;

    void createGBuffer();
    void createDepthFBO();
    void createSSAO();
    void createHDRFBO();
    void renderQuad();
    void render(GameWorld* world, Camera* camera);

    void beginFrame();
    void endFrame();
    void presentFrame();
    void setType(int type)
    {
        this->type = type;
    }
};*/
