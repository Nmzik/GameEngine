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
	static std::unordered_map<std::string, GLuint> TexturesMap;
	static void ChangeBool();
	static GLuint SearchTexture(std::string texture);
	static void LoadTexture(GLuint TextureID);
	static unsigned int loadTexture(std::string path);
};

