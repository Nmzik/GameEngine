#pragma once
#include <iostream>
#include "glm.hpp"
#include <GL/gl3w.h>
#include "stb_image.h"
#include <unordered_map>
#include <string>

struct Texture {
	GLuint TextureID;
	uint32_t referenceCount;
};

class TextureManager
{
public:
	static std::vector<GLuint> TexturesID;
	static std::unordered_map<uint32_t, GLuint> TexturesMap;
	static GLuint GetTexture(uint32_t textureHash);
	static void LoadTexture(uint32_t Hash, GLuint TextureID);
	static void RemoveTexture(uint32_t Hash);
	static GLuint loadTexture(std::string path);

	static void Initialize();

	static GLuint GetTextureID() {
		GLuint TextureID = TexturesID.back();
		TexturesID.pop_back();
		return TextureID;
	}
};

