#pragma once
#include "Mesh.h"
#include "fstream"
#include <iostream>
#include "membuf.h"
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "YdrLoader.h"

class YddLoader
{
public:
	std::vector<YdrLoader *> YdrFiles;
	uint32_t Hash;
	uint32_t time;
	YddLoader(memstream& file, uint32_t hash, btDiscreteDynamicsWorld* world);
	void LoadYdr(uint32_t Hash);
	~YddLoader();

	void Draw();
};

