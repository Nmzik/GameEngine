#pragma once
#include "Model.h"
#include "Player.h"

class GameWorld
{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

public:
	std::vector<Model> models;
	Player* player;

	GameWorld();
	~GameWorld();

	void update();
};

