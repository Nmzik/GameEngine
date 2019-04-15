#pragma once
#include <memory>
#include "Entity.h"
#include "Shader.h"
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include "PhysicsSystem.h"
#include "YftLoader.h"

class CVehicle : public Entity
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
    CVehicle(glm::vec3 position, float mass, YftLoader* yft, btDynamicsWorld* physicsWorld);
    ~CVehicle();
    //std::unique_ptr<btCollisionShape> m_wheelShape;

    glm::mat4 getMat4() const
    {
        return modelMatrix;
    }

    glm::vec3 getPosition() const
    {
        return glm::vec3(m_carChassis->getWorldTransform().getOrigin().getX(),
                         m_carChassis->getWorldTransform().getOrigin().getY(),
                         m_carChassis->getWorldTransform().getOrigin().getZ());
    }

    void setPosition(glm::vec3 position)
    {
        m_carChassis->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
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
