#pragma once
#include <btBulletDynamicsCommon.h>
#include "Model.h"

class Vehicle
{
	unsigned int VBO, EBO, VAO;
public:
	Vehicle(glm::vec3 position, btDiscreteDynamicsWorld* world);
	~Vehicle();

	float throttle;
	float steeringValue;
	btRigidBody* m_carChassis;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle;

	void SetThrottle(float throttle) {
		this->throttle = throttle;
	}

	void SetSteeringValue(float steeringValue) {
		this->steeringValue = steeringValue;
	}

	glm::mat4 GetMat4();
	void PhysicsTick();
	void Draw();
};

