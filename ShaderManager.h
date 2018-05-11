#pragma once
#include <unordered_map>
#include "glew.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	static std::unordered_map<uint32_t, GLuint> ShaderMap;
};

