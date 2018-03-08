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
#include <btBulletDynamicsCommon.h>
#include "YdrLoader.h"

class YbnLoader
{
	btRigidBody * rigidBody;
	btDiscreteDynamicsWorld* CollisionWorld;
	btCompoundShape* compound;
	std::vector<btTriangleMesh*> btMeshes;
	std::vector<btBvhTriangleMeshShape*> btTriangleMeshes;
public:
	uint32_t time;
	uint32_t Hash;
	std::vector<glm::vec3> CenterGeometry;

	YbnLoader(btDiscreteDynamicsWorld* world, memstream& file, uint32_t hash);
	~YbnLoader();
	glm::mat4 GetMat4();
};

