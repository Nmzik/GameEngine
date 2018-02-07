#include "GameWorld.h"

GameWorld::GameWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, -10));

	btOverlappingPairCallback* _overlappingPairCallback = new btGhostPairCallback();
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(_overlappingPairCallback);

	_ResourceManager = new ResourceManager(this);

	player = new Player(glm::vec3(0, 0, 20), dynamicsWorld);

	gameHour = 10;
	gameMinute = 0;

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe |
		btIDebugDraw::DBG_DrawConstraints |
		btIDebugDraw::DBG_DrawConstraintLimits);
	dynamicsWorld->setDebugDrawer(&debug);

	/*for (int i = 0; i < 500; i++) {
		Model model(glm::vec3(0.f, 200.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\cube.obj", "container.jpg", "container2_specular.png", true, true);
		model.Load();
		model.UploadToBuffers();
		models.push_back(model);
		dynamicsWorld->addRigidBody(models[i].getBody());
	}*/

	//LoadYDR(1709559537, glm::vec3(0,0,0), glm::quat(0,0,0,0));
	//LoadYmap(1198958185);
	LoadYmap(710206074);
	LoadYmap(3229005336);
	LoadYmap(324492252);
	LoadYmap(611909151);
	LoadYmap(3871703737);
	LoadYmap(4160496934);
	LoadYmap(1234356306);
	LoadYmap(1560373118);

	YbnLoader loaderybn(dynamicsWorld);
	ybnLoader.push_back(loaderybn);

	//YddLoader testydd;
	//yddLoader.push_back(testydd);

	Model *model = new Model(dynamicsWorld, glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, -0.707f, 0.707f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\plane.obj", nullptr, nullptr, false, true);
	//Model *model1 = new Model(dynamicsWorld, glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\cube.obj", "container.jpg", "container2_specular.png", true, true);
	_ResourceManager->AddToWaitingList(model);
	//_ResourceManager->AddToWaitingList(model1);

	//Model *model2 = new Model(dynamicsWorld, glm::vec3(0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\rungholt\\rungholt.obj", nullptr, nullptr, false, false);
	//_ResourceManager->AddToWaitingList(model2);

	//Model model2(glm::vec3(0.f, 5.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f), glm::vec3(10.0f), "C:\\Users\\nmzik\\Desktop\\skydome\\building1.obj", nullptr, nullptr, false, false);
	//_ResourceManager->AddToWaitingList(model2);

	//dynamicsWorld->addRigidBody(model1.getBody());
}

GameWorld::~GameWorld()
{
	delete dynamicsWorld;
}

void GameWorld::LoadYmap(uint32_t hash)
{
	std::unordered_map<uint32_t, RpfResourceFileEntry>::iterator it;
	it = data.YmapEntries.find(hash);
	if (it != data.YmapEntries.end())
	{
		std::cout << "YMAP Found" << std::endl;
		auto& element = it->second;

		//CAN BE AN ERROR HERE - NOT FULLY IMPLEMENTED!
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		YmapLoader ymap(stream);
		uint32_t testi = 0;

		for (int i = 0; i < ymap.CEntityDefs.size(); i++)
		{
			if (!LoadYDR(ymap.CEntityDefs[i].archetypeName, ymap.CEntityDefs[i].position, glm::quat(ymap.CEntityDefs[i].rotation.x, ymap.CEntityDefs[i].rotation.y, ymap.CEntityDefs[i].rotation.z, ymap.CEntityDefs[i].rotation.w)))
				LoadYDD(ymap.CEntityDefs[i].archetypeName, ymap.CEntityDefs[i].position, glm::quat(ymap.CEntityDefs[i].rotation.x, ymap.CEntityDefs[i].rotation.y, ymap.CEntityDefs[i].rotation.z, ymap.CEntityDefs[i].rotation.w));
		}


		//for (auto& entity : ymap.CEntityDefs)
		//
			//printf("%s ",std::to_string(testi++));
			//LoadYDD(entity.archetypeName);
			//LoadYDR(entity.archetypeName, entity.position);
			//LoadYDD(entity.archetypeName);
			//if ((!LoadYDR(entity.archetypeName)) && (!LoadYDD(entity.archetypeName))) printf("Element Not Found\n");
		//}
	}
	else
	{
		std::cout << "YMAP Not Found" << std::endl;
	}
}

bool GameWorld::LoadYDD(uint32_t hash, glm::vec3 position, glm::quat rotation)
{
	std::unordered_map<uint32_t, RpfResourceFileEntry>::iterator it;
	it = data.YddEntries.find(hash);
	if (it != data.YddEntries.end())
	{
		std::cout << "YDD Found " << it->second.Name << std::endl;
		auto& element = it->second;
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		YddLoader test(stream, position, rotation, hash);
		yddLoader.push_back(test);

		return true;
	}
	else
	{
		//std::cout << "Element Not Found" << std::endl;
		return false;
	}
}

bool GameWorld::LoadYDR(uint32_t hash, glm::vec3 position, glm::quat rotation)
{
	std::unordered_map<uint32_t, RpfResourceFileEntry>::iterator it;
	it = data.YdrEntries.find(hash);
	if (it != data.YdrEntries.end())
	{
		std::cout << "YDR Found " << it->second.Name << std::endl;
		auto& element = it->second;
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);
		printf(" SIZE BUFFER %d MB\n", outputBuffer.size() * sizeof(uint8_t)/1024/1024);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		//YdrLoader test(stream, position);
		ydrLoader.emplace_back(YdrLoader(stream, position, rotation, 0));

		return true;
	}
	else
	{
		//std::cout << "Element Not Found" << std::endl;
		return false;
	}
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
	if (cameraPosition.z < 100.0f) {
		for (int i = 0; i < MaximumAvailablePeds; i++) {
			float xRandom = RandomFloat(cameraPosition.x - radiusTraffic, cameraPosition.x + radiusTraffic);
			float yRandom = RandomFloat(cameraPosition.y - radiusTraffic, cameraPosition.y + radiusTraffic);
			Player *newPlayer = new Player(glm::vec3(xRandom, yRandom, 1), dynamicsWorld);
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
	if (cameraPosition.z < 100.0f) {
		for (int i = 0; i < MaximumAvailableVehicles; i++) {
			float xRandom = RandomFloat(cameraPosition.x - radiusTraffic, cameraPosition.x + radiusTraffic);
			float yRandom = RandomFloat(cameraPosition.y - radiusTraffic, cameraPosition.y + radiusTraffic);
			Vehicle newVehicle(glm::vec3(xRandom, yRandom, 5), dynamicsWorld);
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
