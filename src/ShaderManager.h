#pragma once
#include "opengl.h"
#include <unordered_map>

class ShaderManager
{
	public:
	ShaderManager();
	~ShaderManager();

	static std::unordered_map<uint32_t, GLuint> ShaderMap;
};
