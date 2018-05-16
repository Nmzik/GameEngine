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
	VAOs.resize(15000);
	VBOs.resize(15000);
	EBOs.resize(15000);

	glGenVertexArrays(15000, &VAOs[0]);
	glGenBuffers(15000, &VBOs[0]);
	glGenBuffers(15000, &EBOs[0]);
}
