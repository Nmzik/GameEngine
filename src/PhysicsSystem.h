#pragma once
#include <btBulletDynamicsCommon.h>
#include "PhysicsDebugDrawer.h"

class PhysicsSystem
{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	PhysicsDebugDrawer debug;
public:
	static btDiscreteDynamicsWorld* dynamicsWorld;

	PhysicsSystem();
	~PhysicsSystem();

	void Update(float delta_time);

	PhysicsDebugDrawer& getDebugDrawer() {
		return debug;
	}
};

