#pragma once

#include "glew.h"
#include "TextureManager.h"

class Material
{
public:
	Material(const char* diffuseTexture, const char* specTexture, bool UseSametextureID, GLuint TextureID);
	Material(const char * diffuseTexture, const char * specTexture);
	~Material();

	GLuint diffuseTextureID;
	GLuint normalTextureID;
	GLuint specularTextureID;
	GLuint ambientTextureID;
	GLuint opacityTextureID;

	void bind();
};

