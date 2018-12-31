#pragma once
#include "Mesh.h"

class Model
{

public:
	Model() = default;
	~Model() = default;

	std::vector<Mesh> meshes;
};
