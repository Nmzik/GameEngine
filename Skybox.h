#pragma once
#include "Shader.h"
#include <vector>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class Skybox
{
	Shader* SkyboxShader;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
public:
	Skybox();
	~Skybox();

	unsigned int loadCubemap(std::vector<std::string> faces);
	void Draw();
};

