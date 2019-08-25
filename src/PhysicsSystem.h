#pragma once
#include <memory>
#include "FreeListAllocator.h"
#ifdef WIN32
#include "OpenGL/DebugDrawer.h"
#else
#include "metal/DebugDrawer.h"
#endif
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btAlignedAllocator.h>
#include <glm/vec3.hpp>

class CPed;
class CVehicle;

class PhysicsSystem
{
    std::unique_ptr<btBroadphaseInterface> broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

public:
    PhysicsSystem();
    ~PhysicsSystem();

    DebugDrawer debug;
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

    void putRigidBodyToSleep(btRigidBody* body);
    void wakeRigidBodyFromSleep(btRigidBody* body);

    /*glm::vec3 getGravity() const
    {
        btVector3 gravity = dynamicsWorld->getGravity();
        return glm::vec3(gravity.x(), gravity.y(), gravity.z());
    }*/

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
