#include "GameWorld.h"

GameWorld::GameWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	btOverlappingPairCallback* _overlappingPairCallback = new btGhostPairCallback();
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(_overlappingPairCallback);

	player = new Player(dynamicsWorld);

	gameHour = 10;
	gameMinute = 0;

	for (int i = 0; i < 500; i++) {
		Model model(glm::vec3(0.f, 200.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), "C:\\Users\\nmzik\\Desktop\\cube.obj", "container.jpg", true, true);
		models.push_back(model);
		dynamicsWorld->addRigidBody(models[i].getBody());
	}

	Model model(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), "C:\\Users\\nmzik\\Desktop\\plane.obj", nullptr, false, true);
	models.push_back(model);
	dynamicsWorld->addRigidBody(model.getBody());

	//Model model1(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), "C:\\Users\\nmzik\\Desktop\\rungholt\\rungholt.obj", nullptr, false, false);
	//models.push_back(model1);
	//dynamicsWorld->addRigidBody(model1.getBody());
}

GameWorld::~GameWorld()
{
	delete dynamicsWorld;
}

void GameWorld::update()
{
	dynamicsWorld->stepSimulation(1 / 60.f, 10);
}
