#pragma once

#include <vector>
#include "glm.hpp"
#include "glew.h"
#include "Material.h"

class Mesh
{
	unsigned int VBO, EBO, VAO, normal_buffer;

	Material* material;
	unsigned int num_indices;
	uint32_t num_vertices;
public:
	Mesh();
	Mesh(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals);
	Mesh(std::vector<uint8_t>& vertexData, std::vector<uint16_t>& indices, uint16_t VertexStride);
	~Mesh();

	void Draw();
	void DrawCollision();
};

