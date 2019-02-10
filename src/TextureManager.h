#pragma once

#define OPENGL_NAME_CACHE_SIZE 1024

#include "glm/glm.hpp"
#include "opengl.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class TextureManager
{
	public:
	struct Texture
	{
		GLuint TextureID;
		uint32_t referenceCount;
	};

	static TextureManager& GetTextureManager()
	{
		return texManager;
	}

	GLuint DefaultTexture;

	std::vector<GLuint> TexturesID;
	std::unordered_map<uint32_t, Texture> TexturesMap;
	GLuint GetTexture(uint32_t textureHash);
	void LoadTexture(uint32_t Hash, GLuint TextureID);
	void RemoveTexture(uint32_t Hash);

	GLuint GetTextureID()
	{
		if (TexturesID.size() == 0)
		{
			TexturesID.resize(OPENGL_NAME_CACHE_SIZE);
			glGenTextures(OPENGL_NAME_CACHE_SIZE, &TexturesID[0]);
		}

		GLuint TextureID = TexturesID.back();
		TexturesID.pop_back();

		return TextureID;
	}

	static TextureManager texManager;
};
