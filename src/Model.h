#pragma once
#include "Mesh.h"

class Model
{

public:
	Model();
	~Model();

	std::vector<Mesh> meshes;
};
