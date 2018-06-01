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
	VAOs.resize(40000);
	VBOs.resize(80000);

	glGenVertexArrays(40000, &VAOs[0]);
	glGenBuffers(80000, &VBOs[0]);
}
