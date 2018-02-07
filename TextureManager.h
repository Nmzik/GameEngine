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
	static std::unordered_map<std::string, GLuint> TexturesMap;
	static GLuint SearchTexture(std::string texture);
	static unsigned int loadTexture(std::string path);
};

