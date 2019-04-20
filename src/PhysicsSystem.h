#pragma once
#include <memory>
#include "CPed.h"
#include "FreeListAllocator.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btAlignedAllocator.h>
#include <glm/vec3.hpp>

class PhysicsSystem
{
    std::unique_ptr<btBroadphaseInterface> broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
    //PhysicsDebugDrawer debug;

public:
    PhysicsSystem();
    ~PhysicsSystem();

    void update(float delta_time);

    struct RayResult
    {
        bool HasHit;
        glm::vec3 HitPos;
        glm::vec3 HitNormal;
    };

    RayResult rayCast(glm::vec3& from, glm::vec3& to) const;

    void addPed(CPed* ped);
    void removePed(CPed* ped);
    void addVehicle(CVehicle* vehicle);
    void removeVehicle(CVehicle* vehicle);

    void addRigidBody(btRigidBody* body);
    void removeRigidBody(btRigidBody* body);

    glm::vec3 getGravity() const
    {
        btVector3 gravity = dynamicsWorld->getGravity();
        return glm::vec3(gravity.x(), gravity.y(), gravity.z());
    }

    //DEPRECATED
    btDynamicsWorld* getPhysicsWorld()
    {
        return dynamicsWorld.get();
    }
    /*PhysicsDebugDrawer& getDebugDrawer()
    {
        return debug;
    }*/
};
