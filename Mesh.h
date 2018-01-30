#pragma once

#include <vector>
#include "glew.h"
#include "Material.h"

class Mesh
{
	unsigned int vertex_buffer, EBO, VAO, normal_buffer, tex_coord_buffer, tangent_buffer, bitangent_buffer;

	Material* material;
	unsigned int num_indices;
public:
	Mesh(char const * pathTexture, char const * specTexture);
	Mesh(std::vector<float>& vertices, std::vector<uint16_t>& indices, std::vector<float>& normals, std::vector<float>& texcoords, char const * pathTexture, char const * specTexture);
	Mesh(std::vector<uint8_t>& vertexData, std::vector<uint16_t>& indices, uint16_t VertexStride);
	~Mesh();

	void Draw();
};

