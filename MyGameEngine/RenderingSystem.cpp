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

RenderingSystem::RenderingSystem(SDL_Window* window_) : window{ window_ }, dirLight(glm::vec3(0.1, 0.8, 0.1), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), true)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glcontext = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(0); //ZERO - no vsync

	OpenGL_Init();

	//if (!GLEW_ARB_texture_compression_bptc) printf("NOT INITALIZED BPTC\n");
	//if (!GLEW_EXT_texture_compression_s3tc) printf("NOT INITALIZED s3tc\n");
	//if (!GLEW_EXT_texture_compression_rgtc) printf("NOT INITALIZED rgtc\n");

	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(myDebugCallback, nullptr);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DITHER);

	ScreenResWidth = 1280;
	ScreenResHeight = 720;
	ShadowWidth = 1024;
	ShadowHeight = 1024;

	//skybox = new Skybox();

	SkyboxShader = new Shader("skybox.shader");
	ourShader = new Shader("forward.shader");
	gbuffer = new Shader("gbuffer.shader");
	shaderSSAO = new Shader("ssao.shader");
	shaderSSAOBlur = new Shader("ssao_blur.shader");
	gbufferLighting = new Shader("gbufferLighting.shader");
	DepthTexture = new Shader("DepthTexture.shader");
	hdrShader = new Shader("hdrShader.shader");
	debugDepthQuad = new Shader("debug_quad.shader");

	debugDepthQuad->use();
	debugDepthQuad->setInt("depthMap", 0);

	camera = new Camera();

	createDepthFBO();
	createGBuffer();
	createSSAO();
	createHDRFBO();

	gbuffer->use();
	gbuffer->setInt("DiffuseSampler", 0);
	gbuffer->setInt("BumpSampler", 1);
	gbuffer->setInt("SpecSampler", 2);

	ModelUniformLoc = glGetUniformLocation(gbuffer->ID, "model");
	ViewUniformLoc = glGetUniformLocation(gbuffer->ID, "view");
	ProjUniformLoc = glGetUniformLocation(gbuffer->ID, "projection");

	shaderSSAO->use();
	shaderSSAO->setInt("gDepth", 0);
	shaderSSAO->setInt("gNormal", 1);
	shaderSSAO->setInt("texNoise", 2);
	for (unsigned int i = 0; i < 64; ++i)
		shaderSSAO->setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
	ssaoProjection = glGetUniformLocation(shaderSSAO->ID, "projection");
	ssaoInverseProjectionMatrix = glGetUniformLocation(shaderSSAO->ID, "InverseProjectionMatrix");

	shaderSSAOBlur->use();
	shaderSSAOBlur->setInt("ssaoInput", 0);

	gbufferLighting->use();
	gbufferLighting->setInt("gDepth", 0);
	gbufferLighting->setInt("gNormal", 1);
	gbufferLighting->setInt("gAlbedoSpec", 2);
	gbufferLighting->setInt("shadowMap", 3);
	gbufferLighting->setInt("ssao", 4);

	gbufferLighting->setVec3("light.ambient", dirLight.ambient);
	gbufferLighting->setVec3("light.diffuse", dirLight.diffuse);
	gbufferLighting->setVec3("light.specular", dirLight.specular);


	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 10000.0f);
	InverseProjMatrix = glm::inverse(projection);

	glGenQueries(1, &m_nQueryIDDrawTime);
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

void RenderingSystem::createGBuffer()
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ScreenResWidth, ScreenResHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAlbedoSpec, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, ScreenResWidth, ScreenResHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// create and attach depth buffer (renderbuffer)
	glGenTextures(1, &gDepthMap);
	glBindTexture(GL_TEXTURE_2D, gDepthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, ScreenResWidth, ScreenResHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepthMap, 0);

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::createDepthFBO()
{
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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

void RenderingSystem::createSSAO()
{
	glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ScreenResWidth, ScreenResHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;
	// and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ScreenResWidth, ScreenResHeight, 0, GL_RGB, GL_FLOAT, NULL);
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
		float scale = float(i) / 64.0f;

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

void RenderingSystem::createHDRFBO()
{
	glGenFramebuffers(1, &hdrFBO);
	// create floating point color buffer
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, ScreenResWidth, ScreenResHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ScreenResWidth, ScreenResHeight);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

inline void RenderingSystem::renderQuad()
{
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glBindVertexArray(0);
}

void RenderingSystem::render(GameWorld* world)
{
	double SUNRISE = 5.47f;		// % of Day
	double SUNSET = 19.35f;
	uint8_t MOONRISE = 17;	// % of Day
	uint8_t MOONSET = 4;
	float tod = world->gameHour + world->gameMinute / 60.f;
	if (tod > SUNRISE && tod < SUNSET) {
		double sunT = ((double)tod - SUNRISE) / (SUNSET - SUNRISE);
		float phi = glm::pi<float>() / 2.0f - (float)sunT * glm::pi<float>();
		float theta = 0.0f;

		dirLight.direction = glm::normalize(glm::vec3(-glm::sin(phi)*glm::cos(theta), glm::sin(phi)*glm::sin(theta), glm::cos(phi)));
	}

	/*if (world->gameHour < MOONSET || world->gameHour < MOONRISE) {
		double total = 1.0 - MOONRISE + MOONSET;
		double moonT = (CurrentTime < MOONRISE ? CurrentTime + 1.0 - MOONRISE : CurrentTime - MOONRISE) / total;
		float phi = glm::pi<float>() / 2.0f - (float)moonT * glm::pi<float>();
		float theta = 0.0f;

		MoonDirection = glm::normalize(glm::vec3(-glm::sin(phi)*glm::cos(theta), glm::cos(phi), glm::sin(phi)*glm::sin(theta)));
	}*/

	//glClearColor(0.0, 0.0, 0.0, 0.0);

	glm::mat4 view = camera->GetViewMatrix();
	camera->UpdateFrustum(projection * view);

	if (gpuTimer && !mWaiting)
		glBeginQuery(GL_TIME_ELAPSED, m_nQueryIDDrawTime);

	///geometry to gbuffer->shadowmaps(directional light)->shadowmaps(point light)->ssao->lighting(Final)->skybox
	// --------------------------------GeometryPass Deferred Rendering----------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gbuffer->use();
	gbuffer->setMat4(ViewUniformLoc, view);
	gbuffer->setMat4(ProjUniformLoc, projection);

	/*glDepthMask(GL_FALSE); //SKYDOME IS STATIONARY - SHOULD BE RENDERED LAST - PLAYER CAN GO OUT OF SKYDOME IF HE IS TOO FAR FROM IT
	glm::mat4 SkydomeMatrix = glm::translate(glm::mat4(1.0f), camera->Position) * glm::mat4_cast(glm::quat(-1, 0, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10));
	gbuffer->setMat4(ModelUniformLoc, SkydomeMatrix);

	for (auto &mesh : world->skydome->YdrFiles[2640562617]->meshes)
	{
		glBindVertexArray(mesh.VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);
	}
	glDepthMask(GL_TRUE);*/

	DrawCalls = 0;

	for (auto& ped : world->pedestrians)
	{
		//if ped loaded
		auto& model = ped.getPosition();
		if (camera->intersects(glm::vec3(model[3]), 1.0f)) {
			gbuffer->setMat4(ModelUniformLoc, model);

			for (auto& ydr : ped.playerModel)
			{
				for (auto &mesh : ydr->meshes)
				{
					glBindVertexArray(mesh.VAO);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);
					glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);

					DrawCalls++;
				}
			}
		}
	}

	/*for (int i = 0; i < 20; i++) {
		if (world->pedPool.peds[i].loaded) {
			auto& model = world->pedPool.peds[i].getPosition();
			if (camera->intersects(glm::vec3(model[3]), 1.0f)) {
				gbuffer->setMat4(ModelUniformLoc, model);
				world->pedPool.peds[i].Draw(gbuffer);
			}
		}
	}*/

	for (int i = 0; i < world->vehicles.size(); i++) {
		auto modelVehicle = world->vehicles[i]->GetMat4();

		if (camera->intersects(glm::vec3(modelVehicle[3]), 1.0f)) {
			gbuffer->setMat4(ModelUniformLoc, modelVehicle);
			world->vehicles[i]->Draw(gbuffer);
		}
	}

	for (auto& model : world->renderList)
	{
		for (auto &mesh : model.ydr->meshes)
		{
			gbuffer->setMat4(ModelUniformLoc, model.modelMatrix);

			glBindVertexArray(mesh.VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);
			glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);

			DrawCalls++;
		}
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);

	if (RenderDebugWorld) {
		world->GetDynamicsWorld()->debugDrawWorld();
		gbuffer->setMat4(ModelUniformLoc, glm::mat4(1.0));
		world->getDebugDrawer()->render();
	}

	for (auto& waterMesh : world->WaterMeshes)
	{
		gbuffer->setMat4(ModelUniformLoc, glm::mat4(1.0));
		waterMesh.Draw();

		DrawCalls++;
	}
	glEnable(GL_CULL_FACE);

	//glDisable(GL_CULL_FACE);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//dirLight.direction = glm::rotateX(dirLight.direction, -0.005f);
	// --------------------------------ShadowPass----------------------------------
	/*glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.f, far_plane = 5000.f;
	lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.f, 200.f);
	lightView = glm::lookAt(dirLight.direction + camera->Position, camera->Position, glm::vec3(0, 0, 1));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	DepthTexture->use();
	DepthTexture->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	//glCullFace(GL_BACK);
	
	//glCullFace(GL_FRONT);
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

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, ScreenResWidth, ScreenResHeight);

	// generate SSAO texture
	// ------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	shaderSSAO->use();
	shaderSSAO->setMat4(ssaoProjection, projection);
	shaderSSAO->setMat4(ssaoInverseProjectionMatrix, InverseProjMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gDepthMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	renderQuad();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// blur SSAO texture to remove noise
	// ------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	shaderSSAOBlur->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();

	// --------------------------------LightingPass Deferred Rendering----------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gbufferLighting->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gDepthMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

	gbufferLighting->setVec3("light.direction", dirLight.direction);
	gbufferLighting->setInt("type", 0);
	gbufferLighting->setVec3("viewPos", camera->Position);
	gbufferLighting->setMat4("InverseProjectionMatrix", InverseProjMatrix);
	//gbufferLighting->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	renderQuad();

	// --------------------------------HDR----------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hdrShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	float exposure = 1.0f;
	hdrShader->setInt("UseBlur", 0);
	hdrShader->setFloat("exposure", exposure);
	renderQuad();

	if (gpuTimer) {
		if (!mWaiting) {
			glEndQuery(GL_TIME_ELAPSED);
			mWaiting = true;
		}

		GLint resultReady = 0;
		glGetQueryObjectiv(m_nQueryIDDrawTime, GL_QUERY_RESULT_AVAILABLE, &resultReady);

		if (resultReady)
		{
			glGetQueryObjectuiv(m_nQueryIDDrawTime, GL_QUERY_RESULT, &gpuTime);
			mWaiting = false;
		}
	}
}

void RenderingSystem::skyboxPass()
{
	

	//FORWARD RENDERING
	//skybox->Draw();
}