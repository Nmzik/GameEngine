#pragma once
#include "opengl.h"
#include <vector>

#define OPENGL_NAME_CACHE_SIZE 1024

class MeshManager
{
public:
	/*static MeshManager& GetManager() {
		return manager;
	}*/

	std::vector<GLuint> VAOs;
	std::vector<GLuint> VBOs;

	//GLsizei NextBufferName = OPENGL_NAME_CACHE_SIZE;
	//GLuint BufferNamesCache[OPENGL_NAME_CACHE_SIZE];

	//MeshManager manager;

	void Initialize();
	~MeshManager();

	/*void GenBuffers(GLsizei n, GLuint *buffers)
	{
		if (n < OPENGL_NAME_CACHE_SIZE - NextBufferName)
		{
			memcpy(buffers, &BufferNamesCache[NextBufferName], sizeof(GLuint)*n);
			NextBufferName += n;
		}
		else
		{
			if (n >= OPENGL_NAME_CACHE_SIZE)
			{
				glGenBuffers(n, buffers);
			}
			else
			{
				GLsizei Leftover = OPENGL_NAME_CACHE_SIZE - NextBufferName;

				memcpy(buffers, &BufferNamesCache[NextBufferName], sizeof(GLuint)*Leftover);

				glGenBuffers(OPENGL_NAME_CACHE_SIZE, BufferNamesCache);

				n -= Leftover;
				buffers += Leftover;

				memcpy(buffers, BufferNamesCache, sizeof(GLuint)*n);
				NextBufferName = n;
			}
		}
	}*/

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

