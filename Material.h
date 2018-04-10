#pragma once

#include "glew.h"
#include "TextureManager.h"

class Material
{
public:
	Material(uint32_t Hash);
	~Material();

	GLuint diffuseTextureID;
	GLuint bumpTextureID; //Normal
	GLuint specularTextureID;

	void bind();
};

