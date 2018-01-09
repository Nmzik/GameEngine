#pragma once

#include "glew.h"
#include "stb_image.h"

class Material
{
public:
	Material(const char* diffuseTexture, const char* specTexture);
	~Material();

	GLuint diffuseTextureID;
	GLuint normalTextureID;
	GLuint specularTextureID;
	GLuint ambientTextureID;
	GLuint opacityTextureID;

	unsigned int loadTexture(char const * path);
	void bind();
};

