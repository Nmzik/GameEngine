#pragma once
#include <unordered_map>
#include "opengl.h"

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    static std::unordered_map<uint32_t, GLuint> ShaderMap;
};
