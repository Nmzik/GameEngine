#include "RenderingSystem.h"

void myDebugCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	printf(
		"Source: %d\n"
		"Type:   %d\n"
		"ID:     %d\n"
		"Severity: %d\n"
		"Length:   %d\n"
		"\n%s\n",
		source,
		type,
		id,
		severity,
		length,
		message);
}

RenderingSystem::RenderingSystem(SDL_Window* window_) : window{ window_ }, lightPos(0.0f, 50.0f, 0.0f), sunDirection{ 0.1, 0.8, 0.1 }
{
	glcontext = SDL_GL_CreateContext(window);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);


	//// Enable the debug callback
	//glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback(openglCallbackFunction, nullptr);
	//glDebugMessageControl(
		//GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
	//);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(myDebugCallback, nullptr);

	glEnable(GL_DEPTH_TEST);

	skybox = new Skybox();

	ourShader = new Shader("forward.vs", "forward.fs");
	gbuffer = new Shader("gbuffer.vs", "gbuffer.fs");
	shaderSSAO = new Shader("ssao.vs", "ssao.fs");
	shaderSSAOBlur = new Shader("ssao_blur.vs", "ssao_blur.fs");
	gbufferLighting = new Shader("gbufferLighting.vs", "gbufferLighting.fs");
	DepthTexture = new Shader("DepthTexture.vs", "DepthTexture.fs");
	debugDepthQuad = new Shader("debug_quad.vs", "debug_quad.fs");

	debugDepthQuad->use();
	debugDepthQuad->setInt("depthMap", 0);

	camera = new Camera();

	CreateDepthFBO();
	Create_GBuffer();
	CreateSSAO();

	gbuffer->use();
	gbuffer->setInt("material.diffuse", 0);
	gbuffer->setInt("material.specular", 1);

	shaderSSAO->use();
	shaderSSAO->setInt("gPosition", 0);
	shaderSSAO->setInt("gNormal", 1);
	shaderSSAO->setInt("texNoise", 2);

	shaderSSAOBlur->use();
	shaderSSAOBlur->setInt("ssaoInput", 0);

	gbufferLighting->use();
	gbufferLighting->setInt("gPosition", 0);
	gbufferLighting->setInt("gNormal", 1);
	gbufferLighting->setInt("gAlbedoSpec", 2);
	gbufferLighting->setInt("shadowMap", 3);
	gbufferLighting->setInt("ssao", 4);
	gbufferLighting->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	gbufferLighting->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	gbufferLighting->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
}

RenderingSystem::~RenderingSystem() 
{

}

Camera & RenderingSystem::getCamera()
{
	return *camera;
}


float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void RenderingSystem::Create_GBuffer()
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::CreateDepthFBO()
{
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::CreateSSAO()
{
	glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;
	// and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// generate sample kernel
	// ----------------------
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// generate noise texture
	// ----------------------
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void RenderingSystem::RenderShadowMap()
{
	
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderingSystem::render(GameWorld* world) 
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/////*Deferred Rendering
	///geometry to gbuffer->shadowmaps(directional light)->shadowmaps(point light)->ssao->lighting(Final)->skybox
	////*/
	//GeometryPass Deferred Rendering
	// --------------------------------
	float tod = world->gameHour + world->gameMinute / 60.f;
	float theta = (tod / (60.f * 24.f) - 0.5f) * glm::pi<float>();
	/*glm::vec3 sunDirection{
		sin(theta), 0.0, cos(theta),
	};*/
	sunDirection = glm::normalize(sunDirection);
	
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 5000.0f);
	glm::mat4 view = camera->GetViewMatrix();
	gbuffer->use();
	gbuffer->setMat4("projection", projection);
	gbuffer->setMat4("view", view);

	//light position object
	/*static glm::mat4 model(1.0f);
	model = glm::translate(glm::vec3(0.0f,50.0f,0.0f));
	//model = glm::rotateZ(lightdirection, -0.01f);
	model = glm::rotate(model, -1.f, glm::vec3(0.f, 1.f, 1.f));
	printf("TEST %s\n", glm::to_string(model));
	//model = glm::rotate(-0.01f, lightdirection);
	gbuffer->setMat4("model", model);
	world->models[0].Draw();*/

	for (int i = 0; i < world->models.size(); i++)
	{
		auto model = world->models[i].GetMat4();
		
		//printf("%f\n", glm::distance(camera->Position, glm::vec3(world->models[i].getBody()->getWorldTransform().getOrigin().getX(), world->models[i].getBody()->getWorldTransform().getOrigin().getY(), world->models[i].getBody()->getWorldTransform().getOrigin().getZ())));
		//if (glm::distance(camera->Position, glm::vec3(world->models[i].getBody()->getWorldTransform().getOrigin().getX(), world->models[i].getBody()->getWorldTransform().getOrigin().getY(), world->models[i].getBody()->getWorldTransform().getOrigin().getZ())) < 80.0f) {
		//if (glm::distance(camera->Position, world->models[i].GetPosition()) < 80.0f) {
			gbuffer->setMat4("model", model);
			world->models[i].Draw();
		//}
	}

	auto model = world->player->getPosition();
	gbuffer->setMat4("model", model);
	world->player->Draw();
	//glDisable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	//lightPos.x = camera->Position.x;
	//lightPos.y = camera->Position.y + 50.f;
	//lightPos.z = camera->Position.z;
	sunDirection = glm::rotateZ(sunDirection, -0.01f) / 10;
	//if (lightdirection.y >= 0.1f) lightdirection.y = 0.1f;
	//lightdirection = glm::rotateY(lightdirection, -0.01f);
	//printf("%f %f %f\n", lightdirection.x, lightdirection.y, lightdirection.z);
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.f, far_plane = 5000.f;
	lightProjection = glm::ortho(-x, x, -x, x, -100.f, 200.f);
	glm::vec3 lightInvDir = glm::vec3(2.0f, 0.5f, 2.0f);
	glm::vec3 lightPos = camera->Position + lightInvDir;
	lightView = glm::lookAt(lightPos, camera->Position, glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	DepthTexture->use();
	DepthTexture->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	for (int i = 0; i < world->models.size(); i++)
	{
		auto model = world->models[i].GetMat4();
		//if (glm::distance(camera->Position, world->models[i].GetPosition()) < 80.0f) {
			DepthTexture->setMat4("model", model);
			world->models[i].Draw();
		//}
	}

	//printf("SUN %s\n",glm::to_string(sunDirection).c_str());

	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	debugDepthQuad->use();
	debugDepthQuad->setFloat("near_plane", near_plane);
	debugDepthQuad->setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderQuad();*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);

	//LightingPass Deferred Rendering
	// --------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gbufferLighting->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

	gbufferLighting->setVec3("light.direction", sunDirection);
	gbufferLighting->setVec3("viewPos", camera->Position);
	gbufferLighting->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	// Render quad
	renderQuad();

	SDL_GL_SwapWindow(window);
}

void RenderingSystem::ssaoPass()
{
	// 2. generate SSAO texture
	// ------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 1000.0f);
	shaderSSAO->use();
	// Send kernel + rotation 
	for (unsigned int i = 0; i < 64; ++i)
		shaderSSAO->setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
	shaderSSAO->setMat4("projection", projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	renderQuad();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. blur SSAO texture to remove noise
	// ------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	shaderSSAOBlur->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::skyboxPass()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
	// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
	// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//FORWARD RENDERING
	//skybox->Draw();
}

/*void ForwardRendering() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 1.0, 0.0, 1.0);

	ourShader->use();

	static glm::vec3 lightdirection(0.2f, 0.0f, -0.3f);
	lightdirection.x = 1.0f + sin((SDL_GetTicks() / 1000)) * 2.0f;
	lightdirection.y = sin((SDL_GetTicks() / 1000) / 2.0f) * 1.0f;

	//for all cameras
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 1000.0f);

	glm::mat4 view = camera->GetViewMatrix();
	// pass transformation matrices to the shader
	ourShader->setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	ourShader->setMat4("view", view);
	ourShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	ourShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	ourShader->setVec3("lightDirection", lightdirection);
	ourShader->setVec3("viewPos", camera->Position);

	//light position object
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 1.0f, 2.0f));
	//ourShader->setMat4("model", model);
	//models[0].Draw();

	// render boxes
	for (int i = 0; i < world->models.size(); i++)
	{
		//auto pos = models[i].GetPosition();

		//glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
		//float angle = 20.0f * i;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		auto model = world->models[i].GetMat4();
		ourShader->setMat4("model", model);

		world->models[i].Draw();
	}

	skybox->Draw();

	//renderAllLights


	SDL_GL_SwapWindow(window);
}*/