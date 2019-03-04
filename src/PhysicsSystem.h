#pragma once
#include <memory>
#include "PhysicsDebugDrawer.h"
#include <btBulletDynamicsCommon.h>
#include "FreeListAllocator.h"
#include <LinearMath/btAlignedAllocator.h>

class PhysicsSystem
{
    std::unique_ptr<uint8_t[]> physicsMemory;

    std::unique_ptr<btBroadphaseInterface> broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;

    PhysicsDebugDrawer debug;

public:
    static std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

    PhysicsSystem();
    ~PhysicsSystem();

    void update(float delta_time);

    PhysicsDebugDrawer& getDebugDrawer()
    {
        return debug;
    }
};
