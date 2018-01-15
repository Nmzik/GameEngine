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

	_ResourceManager = new ResourceManager(this);

	player = new Player(dynamicsWorld);

	gameHour = 10;
	gameMinute = 0;

	/*for (int i = 0; i < 500; i++) {
		Model model(glm::vec3(0.f, 200.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\cube.obj", "container.jpg", "container2_specular.png", true, true);
		model.Load();
		//model.UploadToBuffers();
		models.push_back(model);
		dynamicsWorld->addRigidBody(models[i].getBody());
	}*/

	/*model.Load();
	model.UploadToBuffers();
	models.push_back(model);
	dynamicsWorld->addRigidBody(model.getBody());
	//_ResourceManager->AddToWaitingList(model);*/

	Model model(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\plane.obj", nullptr, nullptr, false, true);
	_ResourceManager->AddToWaitingList(model);

	Model model1(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\rungholt\\rungholt.obj", nullptr, nullptr, false, false);
	_ResourceManager->AddToWaitingList(model1);

	//Model model2(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\skydome.obj", nullptr, nullptr, false, false);
	//_ResourceManager->AddToWaitingList(model2);

	//Model model2(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\skydome\\building1.obj", nullptr, nullptr, false, false);
	//_ResourceManager->AddToWaitingList(model2);
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
