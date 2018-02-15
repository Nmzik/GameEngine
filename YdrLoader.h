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
#include "glm/gtx/matrix_decompose.hpp"
#include "membuf.h"

class YdrLoader
{
	glm::mat4 ModelMatrix;
	std::vector<Mesh*> meshes;
	//glm::vec3 Position;
	//glm::quat Rotation;
public:
	uint32_t time;
	bool Loaded = false;
	uint32_t Hash;
	YdrLoader(memstream& file, glm::vec3 position, glm::quat rotation, uint32_t hash);
	~YdrLoader();

	void UploadMeshes();
	glm::mat4& GetMat4();

	void Draw();
};

