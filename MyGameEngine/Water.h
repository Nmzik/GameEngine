#pragma once
#include "GameData.h"

class Water
{
	unsigned int VBO, EBO, VAO;
	unsigned int num_indices;
	GLuint diffuseTextureID;
public:
	Water(WaterQuad waterQuad);
	~Water();
	void Draw();
};

