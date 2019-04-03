#include "GameRenderer.h"

GameRenderer::GameRenderer()
{
    RenderSystem* renderer = new GLRenderSystem();

    RenderContextDescriptor contextDescriptor;
    contextDescriptor.videoMode = {1280, 720};
    renderer->CreateRenderContext(contextDescriptor);

	BufferDescriptor BufDesc;
	Buffer* buffer = renderer->CreateBuffer(BufDesc);

	ShaderDescriptor VertShaderDesc = {ShaderType::Vertex, "testc disk"};
	Shader* vertShader = renderer->CreateShader(VertShaderDesc);
    ShaderDescriptor FragmentShaderDesc = {ShaderType::Fragment, "testc disk"};
        Shader* FragShader = renderer->CreateShader(FragmentShaderDesc);
}

GameRenderer::~GameRenderer()
{
}

void GameRenderer::RenderWorld(GameWorld* world, Camera* curCamera)
{
}

/*#include "CPed.h"
#include "GameWorld.h"
#include "Object.h"
#include "Water.h"
#include "YdrLoader.h"

//#define USE_DX_REVERSE_Z
// GLM_FORCE_LEFT_HANDED
// GLM_FORCE_DEPTH_ZERO_TO_ONE

void myDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
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

GameRenderer::GameRenderer(NativeWindow* window_)
    : window{window_->window}
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    //	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glcontext = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(0);  //	ZERO - no vsync

    OpenGL_Init();

    if (SDL_GL_ExtensionSupported("GL_EXT_texture_filter_anisotropic"))
    {
        float aniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
        printf("ANISO SUPPORTED %f\n", aniso);
    }
    else
    {
        printf("NOT SUPPORTED\n");
    }
    //	if (!GLEW_ARB_texture_compression_bptc) printf("NOT INITALIZED BPTC\n");
    //	if (!GLEW_EXT_texture_compression_s3tc) printf("NOT INITALIZED s3tc\n");
    //	if (!GLEW_EXT_texture_compression_rgtc) printf("NOT INITALIZED rgtc\n");

    //	glEnable(GL_DEBUG_OUTPUT);
    //	glDebugMessageCallback(myDebugCallback, nullptr);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DITHER);

    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);  //	always zero to one

#ifdef USE_DX_REVERSE_Z
    glDepthFunc(GL_GEQUAL);
    glClearDepth(0.0);
#endif  // USE_DX_REVERSE_Z

    SDL_GetWindowSize(window, &ScreenResWidth, &ScreenResHeight);
    ShadowWidth = 1024;
    ShadowHeight = 1024;

#ifdef USE_DX_REVERSE_Z
    const float zNear = 0.01f;
    const float viewAngleVertical = 45.0f;
    float f = 1.0f / tan(viewAngleVertical / 2.0f);
    const float aspect = (float)ScreenResWidth / ScreenResHeight;
    projection = {f / aspect, 0.0f, 0.0f, 0.0f, 0.0f, f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, zNear, 0.0f};
#else
    // projection = glm::perspective(glm::radians(45.0f), (float)ScreenResWidth / (float)ScreenResHeight, 0.1f, 10000.0f);
#endif  // USE_DX_REVERSE_Z

    glGenBuffers(1, &uboGlobal);
    glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &uboModel);
    glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //InverseProjMatrix = glm::inverse(projection);

    //	SkyboxShader = std::make_unique<Shader>("assets/shaders/skybox");
    gbuffer = std::make_unique<Shader>("assets/shaders/gbuffer");
    Transparentgbuffer = std::make_unique<Shader>("assets/shaders/gbuffer2");
    //	shaderSSAO = std::make_unique<Shader>("assets/shaders/ssao");
    //	shaderSSAOBlur = std::make_unique<Shader>("assets/shaders/ssao_blur");
    //gbufferLighting = std::make_unique<Shader>("assets/shaders/gbufferLighting");
    //	DepthTexture = std::make_unique<Shader>("assets/shaders/DepthTexture");
    //	debugDepthQuad = std::make_unique<Shader>("assets/shaders/debug_quad");
    //	hdrShader = std::make_unique<Shader>("assets/shaders/hdrShader");

    //createDepthFBO();
    //createGBuffer();
    /*createSSAO();
	createHDRFBO();

	shaderSSAO->use();
	shaderSSAO->setVec2(2, glm::vec2((float)ScreenResWidth / 4.0, (float)ScreenResHeight / 4.0));
	glUniform3fv(3, 64, glm::value_ptr(ssaoKernel[0]));

	shaderSSAO->setMat4(0, projection);
	shaderSSAO->setMat4(1, InverseProjMatrix);*/

// gbufferLighting->use();
// gbufferLighting->setMat4(0, InverseProjMatrix);

/*hdrShader->use();
	hdrShader->setVec2(3, glm::vec2(1.0f / (float)ScreenResWidth, 1.0f / (float)ScreenResHeight));

    float quadVertices[] = {-1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glGenQueries(1, &m_nQueryIDDrawTime);

    // Black/white checkerboard //Default texture
    glGenTextures(1, &DefaultTexture);
    glBindTexture(GL_TEXTURE_2D, DefaultTexture);
    float pixels[] = {0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
}

GameRenderer::~GameRenderer()
{
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
    ////
    glDeleteQueries(1, &m_nQueryIDDrawTime);
    SDL_GL_DeleteContext(glcontext);
}

void GameRenderer::createGBuffer()
{
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ScreenResWidth, ScreenResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAlbedoSpec, 0);
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ScreenResWidth, ScreenResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);
    // create and attach depth buffer (renderbuffer)
    glGenTextures(1, &gDepthMap);
    glBindTexture(GL_TEXTURE_2D, gDepthMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#ifdef USE_DX_REVERSE_Z
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, ScreenResWidth, ScreenResHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, ScreenResWidth, ScreenResHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
#endif  // USE_DX_REVERSE_Z
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepthMap, 0);

    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameRenderer::createDepthFBO()
{
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
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

void GameRenderer::createSSAO()
{
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ScreenResWidth, ScreenResHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    // and blur stage
    glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ScreenResWidth, ScreenResHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate sample kernel
    // ----------------------
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);  // generates random floats between 0.0 and 1.0
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
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);  // rotate around z-axis (in tangent space)
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

void GameRenderer::createHDRFBO()
{
    glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenResWidth, ScreenResHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
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

inline void GameRenderer::renderQuad()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GameRenderer::render(GameWorld* world, Camera* camera)
{
    beginFrame();

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjection();

    glm::mat4 ProjectionView = projection * view;

    camera->updateFrustum(ProjectionView);

    glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &ProjectionView[0]);

    ///	geometry to gbuffer->shadowmaps(directional light)->shadowmaps(point light)->ssao->lighting(Final)->skybox
    // --------------------------------GeometryPass Deferred Rendering----------------------------------
    //glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef USE_DX_REVERSE_Z
    glClearDepth(0.0);
#endif  // USE_DX_REVERSE_Z
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*glDepthMask(GL_FALSE); //SKYDOME IS STATIONARY - SHOULD BE RENDERED LAST - PLAYER CAN GO OUT OF SKYDOME IF HE IS TOO FAR FROM IT
	glm::mat4 SkydomeMatrix = glm::translate(glm::mat4(1.0f), world->peds[world->currentPlayerID].getPosition()) * glm::mat4_cast(glm::quat(-1, 0, 0, 0))
	* glm::scale(glm::mat4(1.0f), glm::vec3(10.f, 10.f, 10.f));

	glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &SkydomeMatrix[0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (auto& model : world->skydome->YdrFiles.begin()->second->models)
	{
	 for (auto& mesh : model.meshes)
	 {
	  glBindVertexArray(mesh.VAO);
	  glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, DefaultTexture);
	  glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);
	 }
	}
	glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);

    uint32_t curShader = 0;

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboGlobal);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboModel);

    for (auto& GameObject : world->renderList)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &GameObject->modelMatrix[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        for (auto& model : GameObject->ydr->models)
        {
            if ((model.Unk_2Ch & 1) == 0)
            {
                continue;  //	PROXIES
            }
            for (auto& mesh : model.meshes)
            {
                if (curShader != mesh.ShaderName)
                {
                    curShader = mesh.ShaderName;

                    switch (mesh.ShaderName)
                    {
                        case 1:
                            Transparentgbuffer->use();
                            break;

                        default:
                            gbuffer->use();
                            break;
                    }
                }
                glBindVertexArray(mesh.VAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, DefaultTexture);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, DefaultTexture);

                glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);

                DrawCalls++;
            }
        }
    }

    glDisable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DefaultTexture);

    if (RenderDebugWorld)
    {
        PhysicsSystem::dynamicsWorld->debugDrawWorld();

        glm::mat4 DefaultMatrix(1.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &DefaultMatrix[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        world->getDebugDrawer().render();
    }

    /*for (auto& waterMesh : world->WaterMeshes)
	{
	 if (camera->intersects(waterMesh.BSCenter, waterMesh.BSRadius))
	 {
	  gbuffer->setMat4(0, glm::mat4(1.0));
	  
	  glBindVertexArray(VAO);

    //	glActiveTexture(GL_TEXTURE0);
    //	glBindTexture(GL_TEXTURE_2D, diffuseTextureID);

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);

	  DrawCalls++;
	 }
	}*/

/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef USE_DX_REVERSE_Z
    glClearDepth(0.0);
#endif  // USE_DX_REVERSE_Z
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gbufferLighting->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gDepthMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    /*glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);*/

/*gbufferLighting->setVec3(4, world->dirLight.direction);
    gbufferLighting->setVec3(5, world->dirLight.ambient);
    gbufferLighting->setVec3(6, world->dirLight.diffuse);
    gbufferLighting->setVec3(7, world->dirLight.specular);

    gbufferLighting->setVec3(2, camera->getPosition());

    renderQuad();

    /*glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.f, far_plane = 1000.f;
	float scale = 2;
	lightProjection = glm::ortho(-10.0f*scale, 10.0f*scale, -10.0f*scale, 10.0f*scale, near_plane, far_plane);
	lightView = glm::lookAt(world->peds[world->currentPlayerID].getPosition(), world->peds[world->currentPlayerID].getPosition() +
	world->dirLight.direction, glm::vec3(0.0, 0.0, 1.0)); lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	DepthTexture->use();
	DepthTexture->setMat4(0, lightSpaceMatrix);
	//glCullFace(GL_BACK);

	for (auto& GameObject : world->renderList)
	{
	 DepthTexture->setMat4(1, GameObject->getMatrix());

	 for (auto& model : *GameObject->ydr->models)
	 {
	  for (auto& mesh : model.meshes)
	  {
	   glBindVertexArray(mesh.VAO);

	   glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);

	   DrawCalls++;
	  }
	 }
	}

	//glCullFace(GL_FRONT);
	//printf("SUN %s\n",glm::to_string(sunDirection).c_str());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, ScreenResWidth, ScreenResHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	debugDepthQuad->use();
	//debugDepthQuad->setFloat(0, near_plane);
	//debugDepthQuad->setFloat(1, far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderQuad();

    endFrame();

    presentFrame();
}

void GameRenderer::beginFrame()
{
    DrawCalls = 0;

    if (gpuTimer && !mWaiting)
        glBeginQuery(GL_TIME_ELAPSED, m_nQueryIDDrawTime);
}

void GameRenderer::endFrame()
{
    if (gpuTimer)
    {
        if (!mWaiting)
        {
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

void GameRenderer::presentFrame()
{
    SDL_GL_SwapWindow(window);
}

// FULL RENDERING PASS
/*
double SUNRISE = 5.47f; // % of Day
 double SUNSET = 19.35f;
 uint8_t MOONRISE = 17; // % of Day
 uint8_t MOONSET = 4;
 float tod = world->gameHour + world->gameMinute / 60.f;
 if (tod > SUNRISE && tod < SUNSET)
 {
  double sunT = ((double)tod - SUNRISE) / (SUNSET - SUNRISE);
  float phi = glm::pi<float>() / 2.0f - (float)sunT * glm::pi<float>();
  float theta = 0.0f;

  dirLight.direction = glm::normalize(glm::vec3(-glm::sin(phi) * glm::cos(theta), glm::sin(phi) * glm::sin(theta), glm::cos(phi)));
 }

 /*if (world->gameHour < MOONSET || world->gameHour < MOONRISE) {
  double total = 1.0 - MOONRISE + MOONSET;
  double moonT = (CurrentTime < MOONRISE ? CurrentTime + 1.0 - MOONRISE : CurrentTime - MOONRISE) / total;
  float phi = glm::pi<float>() / 2.0f - (float)moonT * glm::pi<float>();
  float theta = 0.0f;

  MoonDirection = glm::normalize(glm::vec3(-glm::sin(phi)*glm::cos(theta), glm::cos(phi), glm::sin(phi)*glm::sin(theta)));
 }*/

//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	MULTIPLE CAMERAS???
/*glm::mat4 view = camera->GetViewMatrix();

glm::mat4 ProjectionView = projection * view;

camera->UpdateFrustum(ProjectionView);

glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);
glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &ProjectionView[0]);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);

if (gpuTimer && !mWaiting)
glBeginQuery(GL_TIME_ELAPSED, m_nQueryIDDrawTime);

///	geometry to gbuffer->shadowmaps(directional light)->shadowmaps(point light)->ssao->lighting(Final)->skybox
// --------------------------------GeometryPass Deferred Rendering----------------------------------
glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
#ifdef USE_DX_REVERSE_Z
glClearDepth(0.0);
#endif // USE_DX_REVERSE_Z
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

gbuffer->use();

DrawCalls = 0;

/*for (auto& vehicle : world->vehicles) {
  auto modelVehicle = vehicle->GetMat4();

  glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &modelVehicle[0]);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  YftLoader* veh = vehicle->GetDrawable();
  if (veh->YdrFile) {
   for (auto& ydr : *veh->YdrFile->models)
   {
    for (auto& mesh : ydr.meshes)
    {
     glBindVertexArray(mesh.VAO);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);
     glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);
    }
   }
  }
 }

 /*for (auto& ped : world->peds)
 {
  //if ped loaded
  auto& model = ped.getMatrix();
  if (camera->intersects(model[3], 1.0f)) {

   glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
   glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &model[0]);
   glBindBuffer(GL_UNIFORM_BUFFER, 0);

   for (auto& ydr : ped.playerModel)
   {
    for (auto& model : *ydr->models)
    {
     for (auto& mesh : model.meshes)
     {
      glBindVertexArray(mesh.VAO);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);

      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, DefaultTexture);

      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, DefaultTexture);

      glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);

      DrawCalls++;
     }
    }
   }
  }
 }

 for (int i = 0; i < m_vehicle->getNumWheels(); i++)
 {
  glm::mat4 model;

  m_vehicle->getWheelTransformWS(i).getOpenGLMatrix(&model[0][0]);
  for (auto& wheel : vehicle->wheels)
  {
   shader->setMat4(3, model);

   for (auto &mesh : *wheel->meshes)
   {
 glBindVertexArray(mesh.VAO);
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);
 glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);
   }

  }
 }
}
 //}
}*/

/*glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboGlobal);
glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboModel);

for (auto& GameObject : world->renderList)
{
 glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
 glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &GameObject->modelMatrix[0]);
 glBindBuffer(GL_UNIFORM_BUFFER, 0);

 for (auto& model : *GameObject->ydr->models)
 {
  for (auto& mesh : model.meshes)
  {
   glBindVertexArray(mesh.VAO);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, DefaultTexture);

   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, DefaultTexture);

   glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);

   DrawCalls++;
  }
 }
}

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, 0);

glDisable(GL_CULL_FACE);

if (RenderDebugWorld)
{
 world->GetDynamicsWorld()->debugDrawWorld();
 gbuffer->setMat4(0, glm::mat4(1.0));
 world->getDebugDrawer()->render();
}

glEnable(GL_CULL_FACE);

//	RENDER SCENE FOR EVERY LIGHT THAT is able CREATE SHADOW
//	for all lights (point lights etc)
//	if light can cast shadowsfu
//	render scene

//	ssr
//	screen space reflection
//	bloom
//	glDisable(GL_CULL_FACE);

//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	dirLight.direction = glm::rotateX(dirLight.direction, -0.005f);
// --------------------------------ShadowPass----------------------------------
/*glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
glViewport(0, 0, 1024, 1024);
glClear(GL_DEPTH_BUFFER_BIT);
glm::mat4 lightProjection, lightView;
glm::mat4 lightSpaceMatrix;
float near_plane = 1.f, far_plane = 5000.f;
lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.f, 200.f);
lightView = glm::lookAt(dirLight.direction + camera->position, camera->position, glm::vec3(0, 0, 1));
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

//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glViewport(0, 0, ScreenResWidth, ScreenResHeight);

// generate SSAO texture
// ------------------------
//	glViewport(0, 0, ScreenResWidth / 2, ScreenResHeight / 2);
/*glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
glClear(GL_COLOR_BUFFER_BIT);
shaderSSAO->use();
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, gDepthMap);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, gNormal);
glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, noiseTexture);
renderQuad();
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);

// blur SSAO texture to remove noise
// ------------------------------------
glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
glClear(GL_COLOR_BUFFER_BIT);
shaderSSAOBlur->use();
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
renderQuad();

// --------------------------------LightingPass Deferred Rendering----------------------------------
//	glViewport(0, 0, ScreenResWidth, ScreenResHeight);
//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef USE_DX_REVERSE_Z
glClearDepth(0.0);
#endif // USE_DX_REVERSE_Z
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

gbufferLighting->use();
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, gDepthMap);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, gNormal);
glActiveTexture(GL_TEXTURE3);
glBindTexture(GL_TEXTURE_2D, depthMap);
glActiveTexture(GL_TEXTURE4);
glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

gbufferLighting->setVec3(4, dirLight.direction);
gbufferLighting->setVec3(5, dirLight.ambient);
gbufferLighting->setVec3(6, dirLight.diffuse);
gbufferLighting->setVec3(7, dirLight.specular);

gbufferLighting->setVec3(2, camera->position);
//	gbufferLighting->setMat4("lightSpaceMatrix", lightSpaceMatrix);
renderQuad();

/*glEnable(GL_DEPTH_CLAMP);

glDepthMask(GL_FALSE); //SKYDOME IS STATIONARY - SHOULD BE RENDERED LAST - PLAYER CAN GO OUT OF SKYDOME IF HE IS TOO FAR FROM IT
glm::mat4 SkydomeMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f)) * glm::mat4_cast(glm::quat(-1, 0, 0, 0)) *
glm::scale(glm::mat4(1.0f), glm::vec3(10000.f, 10000.f, 10000.f)); gbuffer->setMat4(ModelUniformLoc, SkydomeMatrix);

for (auto &mesh : world->skydome->YdrFiles[2640562617]->meshes)
{
 glBindVertexArray(mesh.VAO);
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D, mesh.material.diffuseTextureID);
 glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_SHORT, 0);
}
glDepthMask(GL_TRUE);
glDisable(GL_DEPTH_CLAMP);*/

// --------------------------------HDR----------------------------------
/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef USE_DX_REVERSE_Z
 glClearDepth(0.0);
#endif // USE_DX_REVERSE_Z
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 hdrShader->use();
 glActiveTexture(GL_TEXTURE0);
 /*if (ShowTexture)
  glBindTexture(GL_TEXTURE_2D, TextureManager::GetTexture(1328663666));
 else*/
/*glBindTexture(GL_TEXTURE_2D, colorBuffer);
float exposure = 1.0f;
hdrShader->setInt(1, 0);
hdrShader->setFloat(2, exposure);
renderQuad();


void GameRenderer::skyboxPass()
{
    //	FORWARD RENDERING
}*/