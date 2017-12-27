#pragma once
#include "Mesh.h"
#include <vector>
#include <glm.hpp>
#include "glew.h"
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

	unsigned int VBO, EBO, VAO;

	unsigned int elementsCount;

	btRigidBody* rigidBody;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
public:
	Model(glm::vec3 position, std::vector<float> vertices, bool dynamic);
	~Model();

	btRigidBody* getBody();
	glm::mat4 GetMat4();
	glm::vec3 GetPosition();

	void Draw();
};

