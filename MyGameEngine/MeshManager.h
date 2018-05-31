#pragma once
#include "opengl.h"
#include <vector>

class MeshManager
{
public:
	static std::vector<GLuint> VAOs;
	static std::vector<GLuint> VBOs;

	MeshManager();
	~MeshManager();

	static void Initialize();

	static GLuint GetVAO() {
		GLuint vao = VAOs.back();
		VAOs.pop_back();
		return vao;
	}

	static GLuint GetVBO() {
		GLuint vbo = VBOs.back();
		VBOs.pop_back();
		return vbo;
	}
};

