#pragma once
#include "Mesh.h"

class Model
{

	public:
	Model()  = default;
	~Model() = default;

	uint32_t Unk_2Ch;
	std::vector<Mesh> meshes;
};
