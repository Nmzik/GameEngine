#pragma once
//#include "Game.h"

#include "SDL.h"
#include <random>
#include <iostream>
#include "glew.h"
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "Model.h"
#include "Skybox.h"
#include "Camera.h"
#include "GameWorld.h"

class RenderingSystem
{
private:
	Shader* ourShader;
	Shader* gbuffer;
	Shader* shaderSSAO;
	Shader* shaderSSAOBlur;
	Shader* gbufferLighting;
	Shader* DepthTexture;
	Shader* debugDepthQuad;
	Camera* camera;
	Skybox* skybox;

	unsigned int quadVAO;
	unsigned int quadVBO;
	int type = 0;
	//Game& game;
	glm::mat4 projection;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int ssaoFBO, ssaoBlurFBO;
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
	std::vector<glm::vec3> ssaoKernel;
	unsigned int noiseTexture;
	//G-BUFFER
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;

	void ssaoPass();
	void skyboxPass();

	SDL_Window* window;
	SDL_GLContext glcontext;

public:
	RenderingSystem(SDL_Window* window_);
	~RenderingSystem();

	glm::vec3 lightPos;
	glm::vec3 sunDirection;

	float x = 50.f;
	Camera& getCamera();
	void Create_GBuffer();
	void CreateDepthFBO();
	void CreateSSAO();
	void RenderShadowMap();
	void renderQuad();
	void render(GameWorld* world);

	void setType(int type) {
		this->type = type;
	}
};

