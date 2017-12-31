#pragma once
#include "Mesh.h"
#include <vector>
#include <glm.hpp>
#include "glm/gtc/quaternion.hpp"
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

	btRigidBody* rigidBody;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

public:
	Model(glm::vec3 position, glm::quat rot, std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals, std::vector<float> texcoords, char const * pathTexture, bool dynamic, bool GenerateCollision);
	~Model();

	btRigidBody* getBody();
	glm::mat4 GetMat4();
	glm::vec3 GetPosition();
	unsigned int loadTexture(char const * path);
	void Draw();
};

