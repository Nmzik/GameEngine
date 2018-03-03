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
#include "YtdLoader.h"
#include "YdrLoader.h"

class YftLoader
{
	glm::mat4 ModelMatrix;
public:
	std::vector<Mesh*> meshes;
	uint32_t Hash;

	YftLoader(memstream& file, glm::vec3 position, glm::quat rotation, glm::vec3 scale, uint32_t hash, btDiscreteDynamicsWorld* world);
	~YftLoader();

	glm::mat4& GetMat4();

	void Draw();
};

