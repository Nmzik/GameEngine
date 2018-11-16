#pragma once
//#include "Game.h"

#include "SDL.h"
#include <random>
#include <iostream>
#include "opengl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Skybox.h"
#include "Camera.h"
#include "Light.h"

class GameWorld;

class RenderingSystem
{
private:
	DirectonalLight dirLight;

	std::unique_ptr<Shader> SkyboxShader;

	std::unique_ptr<Shader> gbuffer;
	GLuint ModelUniformLoc;
	GLuint ViewUniformLoc;
	GLuint ProjUniformLoc;

	GLuint ssaoProjection;
	GLuint ssaoInverseProjectionMatrix;

	std::unique_ptr<Shader> shaderSSAO;
	std::unique_ptr<Shader> shaderSSAOBlur;
	std::unique_ptr<Shader> gbufferLighting;
	std::unique_ptr<Shader> DepthTexture;
	std::unique_ptr<Shader> hdrShader;
	std::unique_ptr<Shader> debugDepthQuad;
	std::unique_ptr<Camera> camera;
	Skybox* skybox;

	unsigned int quadVAO;
	unsigned int quadVBO;
	int type = 0;
	//Game& game;
	int ScreenResWidth;
	int ScreenResHeight;
	int ShadowWidth;
	int ShadowHeight;

	glm::mat4 projection;
	glm::mat4 InverseProjMatrix;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int ssaoFBO, ssaoBlurFBO;
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
	std::vector<glm::vec3> ssaoKernel;
	unsigned int noiseTexture;
	///
	unsigned int hdrFBO;
	unsigned int colorBuffer;
	//G-BUFFER
	unsigned int gBuffer;
	unsigned int gNormal, gAlbedoSpec, gDepthMap;

	GLuint m_nQueryIDDrawTime;
	GLuint DefaultTexture;
	void skyboxPass();

	SDL_Window* window;
	SDL_GLContext glcontext;

public:
	RenderingSystem(SDL_Window* window_);
	~RenderingSystem();

	bool ShowTexture = false;
	bool gpuTimer = false;
	bool mWaiting = false;
	GLuint gpuTime = 0;
	uint32_t DrawCalls;
	glm::vec3 lightPos;
	bool RenderDebugWorld = false;
	Camera& getCamera();
	void createGBuffer();
	void createDepthFBO();
	void createSSAO();
	void createHDRFBO();
	void renderQuad();
	void render(GameWorld* world);

	void setType(int type) {
		this->type = type;
	}
};
