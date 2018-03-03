#pragma once
#include <iostream>
#include "glm.hpp"
#include "glew.h"
#include "stb_image.h"
#include <unordered_map>
#include <string>

class TextureManager
{
public:
	static bool testvar;
	static std::unordered_map<uint32_t, GLuint> TexturesMap;
	static void ChangeBool();
	static GLuint GetTexture(uint32_t textureHash);
	static void LoadTexture(uint32_t Hash, GLuint TextureID);
	static void RemoveTexture(uint32_t Hash);
	static GLuint loadTexture(std::string path);
};

