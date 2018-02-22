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
	Mesh(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals);
	Mesh(const uint8_t* meshData, uint64_t VertexPointer, uint32_t VertexSize, uint64_t IndicesPointer, uint32_t IndicesSize, uint16_t VertexStride, uint32_t textureHash);
	~Mesh();

	void Draw();
	void DrawCollision();
};

