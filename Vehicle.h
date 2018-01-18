#pragma once
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Vehicle
{
	unsigned int VBO, EBO, VAO;
public:
	Vehicle(glm::vec3 position, btDiscreteDynamicsWorld* world);
	~Vehicle();

	btRigidBody* m_carChassis;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle;

	glm::mat4 GetMat4();
	void Draw();
};

