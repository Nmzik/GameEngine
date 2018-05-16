#include "MeshManager.h"

std::vector<GLuint> MeshManager::VAOs;
std::vector<GLuint> MeshManager::VBOs;
std::vector<GLuint> MeshManager::EBOs;

MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
}

void MeshManager::Initialize()
{
	VAOs.resize(20000);
	VBOs.resize(20000);
	EBOs.resize(20000);

	glGenVertexArrays(20000, &VAOs[0]);
	glGenBuffers(20000, &VBOs[0]);
	glGenBuffers(20000, &EBOs[0]);
}
