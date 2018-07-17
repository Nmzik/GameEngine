#pragma once
#include "opengl.h"
#include <vector>

class MeshManager
{
public:
	static MeshManager& GetManager() {
		static MeshManager manager;
		return manager;
	}

	std::vector<GLuint> VAOs;
	std::vector<GLuint> VBOs;

	MeshManager();
	~MeshManager();

	GLuint GetVAO() {
		GLuint vao = VAOs.back();
		VAOs.pop_back();
		return vao;
	}

	GLuint GetVBO() {
		GLuint vbo = VBOs.back();
		VBOs.pop_back();
		return vbo;
	}
};

