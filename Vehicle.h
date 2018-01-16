#pragma once
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Vehicle
{
	btRigidBody* m_carChassis;
	unsigned int VBO, EBO, VAO;
public:
	Vehicle(btDiscreteDynamicsWorld* world);
	~Vehicle();

	btRaycastVehicle* m_vehicle;

	glm::mat4 GetMat4();
	void Draw();
};

