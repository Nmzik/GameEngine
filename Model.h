#pragma once
#include "Mesh.h"
#include <vector>
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glew.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include <btBulletDynamicsCommon.h>

typedef struct
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
}
Vertex;

class Model
{
	std::vector<Mesh> meshes;

	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;

	btDiscreteDynamicsWorld* world;
	btRigidBody* rigidBody;
	glm::vec3 position;
	glm::quat rot;
	glm::vec3 scale;

	bool dynamic = false;

	glm::mat4 ModelMatrix;

	char const * ModelPath;
	char const * pathTexture;
	char const * specTexture;

public:
	Model(btDiscreteDynamicsWorld* DynamicsWorld, glm::vec3 position, glm::quat rot, glm::vec3 scale, char const * ModelPath, char const * pathTexture, const char* specTexture, bool dynamic, bool GenerateCollision);
	~Model();

	float xpos = 0;
	bool GenerateCollision = true;
	bool isLoaded = false;
	btRigidBody* getBody();
	glm::mat4 getModelMatrix();
	glm::vec3 GetPosition();
	void Load();
	void UploadToBuffers();
	void Draw();
};

