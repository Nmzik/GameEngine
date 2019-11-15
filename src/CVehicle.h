#pragma once
#include <memory>

#include "CEntity.h"
#include <btBulletDynamicsCommon.h>

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "loaders/YftLoader.h"

class CVehicle : public CEntity
{
    YftLoader* vehicle;
    //
    std::unique_ptr<btCollisionShape> chassisShape;
    std::unique_ptr<btCompoundShape> compound;
    //
    std::unique_ptr<btRigidBody> m_carChassis;
    //
    std::unique_ptr<btVehicleRaycaster> m_vehicleRayCaster;
    std::unique_ptr<btRaycastVehicle> m_vehicle;

    float throttle;
    float steeringValue;

public:
    CVehicle(glm::vec3 position, glm::quat rot, float mass, YftLoader* yft, btDynamicsWorld* physicsWorld);
    ~CVehicle();
    //std::unique_ptr<btCollisionShape> m_wheelShape;

    void setPosition(glm::vec3 pos)
    {
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

        m_carChassis->setWorldTransform(transform);
    }

    btRaycastVehicle* getRaycastVehicle() const
    {
        return m_vehicle.get();
    }

    btRigidBody* getCarChassisRigidbody() const
    {
        return m_carChassis.get();
    }

    YftLoader* getDrawable() const
    {
        return vehicle;
    }

    void setThrottle(float throttle)
    {
        this->throttle = throttle;
    }

    void setSteeringValue(float steeringValue)
    {
        this->steeringValue = steeringValue;
    }

    void physicsTick();
};
