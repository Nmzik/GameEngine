#pragma once
#include "Mesh.h"
#include "fstream"
#include <iostream>
#include <algorithm>
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "membuf.h"
#include "YdrLoader.h"

class YftLoader
{
	YdrLoader* YdrFile;
public:
	uint32_t time;

	YftLoader(memstream& file, btDiscreteDynamicsWorld* world);
	~YftLoader();

	void Draw();
};

