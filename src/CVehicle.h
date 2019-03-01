#pragma once
#include <memory>
#include "Entity.h"
#include "Shader.h"
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

class YftLoader;

class CVehicle : public Entity
{
    YftLoader* vehicle;

public:
    CVehicle(glm::vec3 position, float mass, YftLoader* yft);
    ~CVehicle();

    glm::mat4 GetMat4();

    float throttle;
    float steeringValue;

    std::unique_ptr<btRigidBody> m_carChassis;
    std::unique_ptr<btCollisionShape> chassisShape;
    std::unique_ptr<btCompoundShape> compound;
    std::unique_ptr<btVehicleRaycaster> m_vehicleRayCaster;
    std::unique_ptr<btRaycastVehicle> m_vehicle;
    std::unique_ptr<btCollisionShape> m_wheelShape;
    std::unique_ptr<btDefaultMotionState> myMotionState;

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
