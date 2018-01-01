#pragma once
//#include "Game.h"

#include "SDL.h"
#include "glew.h"
#include "glm.hpp"
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
	Shader* gbufferLighting;
	Shader* DepthTexture;
	Shader* debugDepthQuad;
	Camera* camera;
	Skybox* skybox;
	//Game& game;

	unsigned int depthMapFBO;
	unsigned int depthMap;
	//G-BUFFER
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;

	SDL_Window* window;
	SDL_GLContext glcontext;

public:
	RenderingSystem(SDL_Window* window_);
	~RenderingSystem();

	float x = 50.f;
	Camera& getCamera();
	void Create_GBuffer();
	void CreateDepthFBO();
	void RenderShadowMap();
	void update(GameWorld* world);

};

