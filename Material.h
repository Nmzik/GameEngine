#pragma once

#include "glew.h"
#include "TextureManager.h"

class Material
{
public:
	Material(uint32_t Hash);
	~Material();

	GLuint diffuseTextureID;
	GLuint normalTextureID;
	GLuint specularTextureID;
	GLuint ambientTextureID;
	GLuint opacityTextureID;

	void bind();
};

