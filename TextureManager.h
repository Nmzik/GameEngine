#pragma once
#include <iostream>
#include "glm.hpp"
#include "glew.h"
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
	static std::unordered_map<uint32_t, Texture> TexturesMap;
	static GLuint GetTexture(uint32_t textureHash);
	static void LoadTexture(uint32_t Hash, GLuint TextureID);
	static void RemoveTexture(uint32_t Hash);
	static GLuint loadTexture(std::string path);
};

