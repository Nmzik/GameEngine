#include "MeshManager.h"

MeshManager::MeshManager()
{
	VAOs.resize(40000);
	VBOs.resize(80000);

	glGenVertexArrays(40000, &VAOs[0]);
	glGenBuffers(80000, &VBOs[0]);
}


MeshManager::~MeshManager()
{
	glDeleteBuffers(80000, &VBOs[0]);
	glDeleteVertexArrays(40000, &VAOs[0]);
}
