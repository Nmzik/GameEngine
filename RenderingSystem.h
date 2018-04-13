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
#include "glm/gtx/matrix_decompose.hpp"

#include "Shader.h"
#include "Skybox.h"
#include "Camera.h"
#include "GameWorld.h"
#include "Light.h"

class RenderingSystem
{
private:
	DirectonalLight dirLight;

	Shader* SkyboxShader;
	Shader* ourShader;
	Shader* gbuffer;
	Shader* shaderSSAO;

	GLuint ssaoProjection;
	GLuint ssaoInverseProjectionMatrix;

	Shader* shaderSSAOBlur;
	Shader* gbufferLighting;
	Shader* DepthTexture;
	Shader* hdrShader;
	Shader* debugDepthQuad;
	Camera* camera;
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

	void skyboxPass();

	SDL_Window* window;
	SDL_GLContext glcontext;

public:
	RenderingSystem(SDL_Window* window_);
	~RenderingSystem();

	glm::vec3 lightPos;
	bool DrawCollision = false;
	bool test = true;
	bool RenderDebugWorld = false;
	bool hdrEnabled = true;
	float x = 50.f;
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

