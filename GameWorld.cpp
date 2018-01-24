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

	//YdrLoader test;

	player = new Player(glm::vec3(-50, 20, 0), dynamicsWorld);

	gameHour = 10;
	gameMinute = 0;

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe |
		btIDebugDraw::DBG_DrawConstraints |
		btIDebugDraw::DBG_DrawConstraintLimits);
	dynamicsWorld->setDebugDrawer(&debug);

	/*for (int i = 0; i < 20; i++) {
		createPedestrian();
	}*/

	//vehicles.reserve(20);

	for (int i = 0; i < 20; i++) {
		createVehicle();
	}
	/*for (int i = 0; i < 500; i++) {
		Model model(glm::vec3(0.f, 200.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\cube.obj", "container.jpg", "container2_specular.png", true, true);
		model.Load();
		model.UploadToBuffers();
		models.push_back(model);
		dynamicsWorld->addRigidBody(models[i].getBody());
	}*/

	Model model(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\plane.obj", nullptr, nullptr, false, true);
	_ResourceManager->AddToWaitingList(model);
	Model model1(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\cube.obj", "container.jpg", "container2_specular.png", true, true);
	_ResourceManager->AddToWaitingList(model1);

	//Model model2(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\rungholt\\rungholt.obj", nullptr, nullptr, false, false);
	//_ResourceManager->AddToWaitingList(model2);

	Model model2(glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(10.0f), "C:\\Users\\nmzik\\Desktop\\skydome\\building1.obj", nullptr, nullptr, false, false);
	_ResourceManager->AddToWaitingList(model2);

	//dynamicsWorld->addRigidBody(model1.getBody());
}

GameWorld::~GameWorld()
{
	delete dynamicsWorld;
}

void GameWorld::createPedestrian()
{
	Player *newPlayer = new Player(glm::vec3(0,20,0), dynamicsWorld);
	pedestrians.push_back(newPlayer);
}

void GameWorld::createVehicle()
{
	Vehicle newVehicle(glm::vec3(-20,0,0), dynamicsWorld);
	vehicles.push_back(newVehicle);
}

void GameWorld::Update()
{
	for (auto& pedestrian : pedestrians)
	{
		pedestrian->PhysicsTick();
	}

	for (auto& vehicle : vehicles)
	{
		vehicle.PhysicsTick();
	}
}

float RandomFloat(float min, float max) {
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

void GameWorld::UpdateTraffic(glm::vec3 cameraPosition)
{
	float radiusTraffic = 20.0f;
	//PEDESTRIANS
	for (int i = 0; i < pedestrians.size(); i++) {
		glm::vec3 pedestrianPosition(pedestrians[i]->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), pedestrians[i]->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), pedestrians[i]->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ());
		if (glm::distance(cameraPosition, pedestrianPosition) >= 100.0f) {
			dynamicsWorld->removeCharacter(pedestrians[i]->getPhysCharacter());
			dynamicsWorld->removeCollisionObject(pedestrians[i]->getPhysCharacter()->getGhostObject());
			//delete pedestrians[i];
			pedestrians.erase(pedestrians.begin() + i);
		}
	}

	int MaximumAvailablePeds = 20 - pedestrians.size(); //HARDCODED
	if (cameraPosition.y < 100.0f) {
		for (int i = 0; i < MaximumAvailablePeds; i++) {
			float xRandom = RandomFloat(cameraPosition.x - radiusTraffic, cameraPosition.x + radiusTraffic);
			float zRandom = RandomFloat(cameraPosition.z - radiusTraffic, cameraPosition.z + radiusTraffic);
			Player *newPlayer = new Player(glm::vec3(xRandom, 0, zRandom), dynamicsWorld);
			pedestrians.push_back(newPlayer);
		}
	}
	//CARS
	for (int i = 0; i < vehicles.size(); i++) {
		glm::vec3 vehiclePosition(vehicles[i].m_carChassis->getWorldTransform().getOrigin().getX(), vehicles[i].m_carChassis->getWorldTransform().getOrigin().getY(), vehicles[i].m_carChassis->getWorldTransform().getOrigin().getZ());
		if (glm::distance(cameraPosition, vehiclePosition) >= 100.0f) {
			dynamicsWorld->removeVehicle((vehicles[i].m_vehicle));
			dynamicsWorld->removeRigidBody((vehicles[i].m_carChassis));
			vehicles.erase(vehicles.begin() + i);
		}
	}

	int MaximumAvailableVehicles = 10 - vehicles.size(); //HARDCODED
	if (cameraPosition.y < 100.0f) {
		for (int i = 0; i < MaximumAvailableVehicles; i++) {
			float xRandom = RandomFloat(cameraPosition.x - radiusTraffic, cameraPosition.x + radiusTraffic);
			float zRandom = RandomFloat(cameraPosition.z - radiusTraffic, cameraPosition.z + radiusTraffic);
			Vehicle newVehicle(glm::vec3(xRandom, 1, zRandom), dynamicsWorld);
			vehicles.push_back(newVehicle);
		}
	}
}

Vehicle* GameWorld::FindNearestVehicle()
{
	float d = 15.0f;

	Vehicle* nearestVehicle = nullptr;

	for (auto& vehicle: vehicles)
	{
		glm::vec3 PlayerPosition(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ());
		glm::vec3 VehiclePosition(vehicle.m_carChassis->getWorldTransform().getOrigin().getX(), vehicle.m_carChassis->getWorldTransform().getOrigin().getY(), vehicle.m_carChassis->getWorldTransform().getOrigin().getZ());
		float vd = glm::length(PlayerPosition - VehiclePosition);
		if (vd < d) {
			d = vd;
			nearestVehicle = &vehicle;
		}
	}

	return nearestVehicle;
}

void GameWorld::DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection)
{
	glm::vec3 HitPos = CameraPosition + lookDirection * 100;
	btVector3 from(CameraPosition.x, CameraPosition.y, CameraPosition.z),
		to(HitPos.x, HitPos.y, HitPos.z);
	btCollisionWorld::ClosestRayResultCallback cb(from, to);
	cb.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	dynamicsWorld->rayTest(from, to, cb);

	if (cb.hasHit()) {
		
		Player* player = static_cast<Player*>(cb.m_collisionObject->getUserPointer());
		//printf("player pointer %p\n", (void*)&player);
		if (player != nullptr) {
			player->TakeDamage(20);
			player->getPhysCharacter()->applyImpulse(btVector3(0.0f, 0.0f, -20.0f));
			if (!player->isAlive()) {
				printf("KILLED!\n");
				//fun stuff
				//player->Jump();
				//ACTUALLY DONT DELETE PEDESTRIAN/PLAYER (MESH SHOULD BE STAYED ) - for example ambulance coming to help him etc 
				//DELETEING WILL BE DONE BY TRAFFIC UPDATER (CAMERA FAR FROM PEDESTRIAN -> DELETE!)
			}
		}
		//cb.setUserPointer
	}
}



void GameWorld::update()
{
	Update();
	dynamicsWorld->stepSimulation(1 / 60.f, 10);
}
