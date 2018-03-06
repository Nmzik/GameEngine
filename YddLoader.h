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
	std::vector<uint32_t> Hashes;
	std::vector<Mesh*> meshes;
	glm::mat4 ModelMatrix;
public:
	uint32_t Hash;
	YddLoader(memstream& file, glm::vec3 position, glm::quat rotation, glm::vec3 scale, uint32_t hash);
	~YddLoader();

	glm::mat4& GetMat4();

	void Draw();
};

