#pragma once

#include "opengl.h"
#include "Shader.h"
#include "TextureManager.h"

class Material
{
public:
	Material(uint32_t DiffuseHash, uint32_t BumpHash, uint32_t SpecularHash, uint32_t DetailHash);
	~Material();

	bool useBump = false;

	GLuint diffuseTextureID;
	GLuint bumpTextureID; //Normal
	GLuint specularTextureID;
	GLuint detailTextureID;
};

