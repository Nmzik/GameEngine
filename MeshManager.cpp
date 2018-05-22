#include "MeshManager.h"

std::vector<GLuint> MeshManager::VAOs;
std::vector<GLuint> MeshManager::VBOs;

MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
}

void MeshManager::Initialize()
{
	VAOs.resize(20000);
	VBOs.resize(40000);

	glGenVertexArrays(20000, &VAOs[0]);
	glGenBuffers(40000, &VBOs[0]);
}
