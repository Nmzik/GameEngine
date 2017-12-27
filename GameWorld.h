#pragma once
#include "Model.h"

class GameWorld
{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

public:
	btRigidBody * groundRigidBody;
	std::vector<Model> models;

	GameWorld();
	~GameWorld();

	void update();
};

