#pragma once
#include <memory>
#include "FreeListAllocator.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btAlignedAllocator.h>

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

	btDiscreteDynamicsWorld* getDynamicsWorld() const {
            return dynamicsWorld.get();
	}
    /*PhysicsDebugDrawer& getDebugDrawer()
    {
        return debug;
    }*/
};
