#pragma once
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "YftLoader.h"
#include "Shader.h"

class Vehicle
{
	YftLoader* vehicle;
public:
	Vehicle(glm::vec3 position, float mass, YftLoader* yft, btDiscreteDynamicsWorld* world);
	~Vehicle();

	float throttle;
	float steeringValue;
	btRigidBody* m_carChassis;
	btCollisionShape* chassisShape;
	btCompoundShape* compound;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle;
	btCollisionShape* m_wheelShape;

	void SetThrottle(float throttle) {
		this->throttle = throttle;
	}

	void SetSteeringValue(float steeringValue) {
		this->steeringValue = steeringValue;
	}

	glm::mat4 GetMat4();
	void PhysicsTick();
	void Draw(Shader* shader);
};

