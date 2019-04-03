#pragma once
#include <vector>

class Shader;

struct ShaderProgramDescriptor
{
	Shader* vertexShader = nullptr;
	Shader* fragmentShader = nullptr;
};

class ShaderProgram
{
public:
	ShaderProgram();
	virtual ~ShaderProgram();
};

