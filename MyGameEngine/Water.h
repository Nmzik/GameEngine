#pragma once
#include "GameData.h"

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

