#pragma once

#include <vector>
#include "glew.h"
#include "stb_image.h"

class Mesh
{
	unsigned int vertex_buffer, EBO, VAO, normal_buffer, tex_coord_buffer;

	unsigned int Texture;
	unsigned int num_indices;
public:
	Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, std::vector<float>& normals, std::vector<float>& texcoords, char const * pathTexture);
	~Mesh();

	unsigned int loadTexture(char const * path);
	void Draw();
};

