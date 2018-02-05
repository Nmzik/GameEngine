#pragma once

#include "Mesh.h"
#include "fstream"
#include <iostream>
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "membuf.h"

class YdrLoader
{
	std::vector<Mesh> meshes;
public:

	YdrLoader(memstream& file);
	~YdrLoader();

	glm::mat4 GetMat4();

	void Draw();
};

