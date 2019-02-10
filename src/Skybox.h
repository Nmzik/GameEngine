#pragma once
#include "Shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

class Skybox
{
	Shader* SkyboxShader;
	unsigned int vertex_buffer, EBO, VAO, normal_buffer, tex_coord_buffer, tangent_buffer, bitangent_buffer;
	int num_indices;
	unsigned int cubemapTexture;

	public:
	Skybox();
	~Skybox();

	unsigned int loadCubemap(std::vector<std::string> faces);
	void Draw();
};
