#pragma once
#include "GameData.h"
#include "opengl.h"
#include "MeshManager.h"
#include "TextureManager.h"

class Water
{
	unsigned int VBO, EBO, VAO;
	unsigned int num_indices;
	GLuint diffuseTextureID;
public:
	glm::vec3 BSCenter;
	float BSRadius;

	Water(WaterQuad waterQuad);
	~Water();
	void Draw();
};

