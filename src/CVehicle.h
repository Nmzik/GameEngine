#pragma once
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "YftLoader.h"
#include "Shader.h"
#include "Entity.h"
#include "YdrLoader.h"
#include "PhysicsSystem.h"

class CVehicle : public Entity
{
	YftLoader* vehicle;

public:
	CVehicle(glm::vec3 position, float mass, YftLoader* yft);
	~CVehicle();

	glm::mat4 GetMat4();

	float throttle;
	float steeringValue;
	btRigidBody* m_carChassis;
	btCollisionShape* chassisShape;
	btCompoundShape* compound;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle;
	btCollisionShape* m_wheelShape;
	YftLoader* GetDrawable()
	{
		return vehicle;
	}

	void SetThrottle(float throttle)
	{
		this->throttle = throttle;
	}

	void SetSteeringValue(float steeringValue)
	{
		this->steeringValue = steeringValue;
	}

	void PhysicsTick();
};
