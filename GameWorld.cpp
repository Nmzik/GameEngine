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

	//Default texture
	TextureManager::LoadTexture(0, TextureManager::loadTexture("blank.jpg"));
	//////////////////////////////////////////////////////////////////////////////

	_ResourceManager = new ResourceManager(this);

	for (auto& mapNode : cacheFile.AllMapNodes)
	{
		spaceGrid.AddMapNode(&mapNode);
	}

	for (auto& BoundsItem : cacheFile.AllBoundsStoreItems)
	{
		spaceGrid.AddBoundsItem(&BoundsItem);
	}

	for (auto& CInteriorProxy : cacheFile.AllCInteriorProxies)
	{
		spaceGrid.AddCInteriorProxy(&CInteriorProxy);
	}

	player = new Player(glm::vec3(0, 0, 20), dynamicsWorld);

	gameHour = 10;
	gameMinute = 0;

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe |
		btIDebugDraw::DBG_DrawConstraints |
		btIDebugDraw::DBG_DrawConstraintLimits);
	dynamicsWorld->setDebugDrawer(&debug);

	/*std::unordered_map<uint32_t, CMloArchetypeDef>::iterator it;
	it = data.MloDictionary.find(210892389);
	if (it != data.MloDictionary.end())
	{
		printf("");
	}
	else {
		printf("");
	}

	it = data.MloDictionary.find(1681413451);
	if (it != data.MloDictionary.end())
	{
		printf("");
	}
	else {
		printf("");
	}*/
	//LoadYDR(3225204062, glm::vec3(), glm::quat());
	/*for (int i = 0; i < 500; i++) {
		Model model(glm::vec3(0.f, 200.f, 0.f), glm::quat(0.f, 0.f, 0.f, 1.f), glm::vec3(1.0f), "C:\\Users\\nmzik\\Desktop\\cube.obj", "container.jpg", "container2_specular.png", true, true);
		model.Load();
		model.UploadToBuffers();
		models.push_back(model);
		dynamicsWorld->addRigidBody(models[i].getBody());
	}*/

	
}

GameWorld::~GameWorld()
{
	delete dynamicsWorld;
}

void GameWorld::LoadYmap(uint32_t hash, glm::vec3 cameraPosition)
{
	auto it = std::find_if(ymapLoader.begin(), ymapLoader.end(), [hash](YmapLoader* item) -> bool { return item->Hash == hash; });
	if (it != ymapLoader.end()) {
		YmapLoader* map = (*it);
		//printf("FOUND SAME\n");
		//FOUND
		map->time = SDL_GetTicks() / 1000;
		if (!(map->flags & 1) > 0) { //DONT LOAD SCRIPTED MAPS
			for (int i = 0; i < map->CEntityDefs.size(); i++)
			{
				if (map->CEntityDefs[i].lodLevel == 0 || map->CEntityDefs[i].lodLevel == 4 || map->CEntityDefs[i].lodLevel == 5) {
					if (glm::distance(cameraPosition, map->CEntityDefs[i].position) <= map->CEntityDefs[i].lodDist) {
						LoadYTD(map->CEntityDefs[i].archetypeName);
						if (!LoadYDR(map->CEntityDefs[i].archetypeName, map->CEntityDefs[i].position, glm::quat(-map->CEntityDefs[i].rotation.w, map->CEntityDefs[i].rotation.x, map->CEntityDefs[i].rotation.y, map->CEntityDefs[i].rotation.z), glm::vec3(map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleZ)))
							if (!LoadYDD(map->CEntityDefs[i].archetypeName, map->CEntityDefs[i].position, glm::quat(-map->CEntityDefs[i].rotation.w, map->CEntityDefs[i].rotation.x, map->CEntityDefs[i].rotation.y, map->CEntityDefs[i].rotation.z), glm::vec3(map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleZ)))
								LoadYFT(map->CEntityDefs[i].archetypeName, map->CEntityDefs[i].position, glm::quat(-map->CEntityDefs[i].rotation.w, map->CEntityDefs[i].rotation.x, map->CEntityDefs[i].rotation.y, map->CEntityDefs[i].rotation.z), glm::vec3(map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleZ));
					}
				}
			}
		}
	}
	else {
		std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
		it = data.YmapEntries.find(hash);
		if (it != data.YmapEntries.end())
		{
			//std::cout << "YMAP Found" << std::endl;
			auto& element = *(it->second);

			//CAN BE AN ERROR HERE - NOT FULLY IMPLEMENTED!
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);

			memstream stream(outputBuffer.data(), outputBuffer.size());

			YmapLoader *ymap = new YmapLoader(stream, hash);
			ymap->time = SDL_GetTicks() / 1000;
			ymapLoader.push_back(ymap);

			if (!(ymap->flags & 1) > 0) { //DONT LOAD SCRIPTED MAPS
				for (int i = 0; i < ymap->CEntityDefs.size(); i++)
				{
					if (ymap->CEntityDefs[i].lodLevel == 0 || ymap->CEntityDefs[i].lodLevel == 4 || ymap->CEntityDefs[i].lodLevel == 5) {
						if (glm::distance(cameraPosition, ymap->CEntityDefs[i].position) <= ymap->CEntityDefs[i].lodDist) {
							LoadYTD(ymap->CEntityDefs[i].archetypeName);

							if (!LoadYDR(ymap->CEntityDefs[i].archetypeName, ymap->CEntityDefs[i].position, glm::quat(-ymap->CEntityDefs[i].rotation.w, ymap->CEntityDefs[i].rotation.x, ymap->CEntityDefs[i].rotation.y, ymap->CEntityDefs[i].rotation.z), glm::vec3(ymap->CEntityDefs[i].scaleXY, ymap->CEntityDefs[i].scaleXY, ymap->CEntityDefs[i].scaleZ)))
								if(!LoadYDD(ymap->CEntityDefs[i].archetypeName, ymap->CEntityDefs[i].position, glm::quat(-ymap->CEntityDefs[i].rotation.w, ymap->CEntityDefs[i].rotation.x, ymap->CEntityDefs[i].rotation.y, ymap->CEntityDefs[i].rotation.z), glm::vec3(ymap->CEntityDefs[i].scaleXY, ymap->CEntityDefs[i].scaleXY, ymap->CEntityDefs[i].scaleZ)))
									LoadYFT(ymap->CEntityDefs[i].archetypeName, ymap->CEntityDefs[i].position, glm::quat(-ymap->CEntityDefs[i].rotation.w, ymap->CEntityDefs[i].rotation.x, ymap->CEntityDefs[i].rotation.y, ymap->CEntityDefs[i].rotation.z), glm::vec3(ymap->CEntityDefs[i].scaleXY, ymap->CEntityDefs[i].scaleXY, ymap->CEntityDefs[i].scaleZ));
						}
					}
				}
			}
		}
		else
		{
			//std::cout << "YMAP Not Found" << std::endl;
		}
	}
}

bool GameWorld::LoadYTYP(uint32_t hash)
{
	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
	it = data.YtypEntries.find(hash);
	if (it != data.YtypEntries.end())
	{
		std::cout << "YTYP Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		YtypLoader* file = new YtypLoader(stream);
		ytypLoader.push_back(file);

		return true;
	}
	else
	{
		//std::cout << "Element Not Found" << std::endl;
		return false;
	}
}

bool GameWorld::LoadYTD(uint32_t hash)
{
	for (auto& ytdFile : ytdLoader)
	{
		if (ytdFile->Hash == hash) {
			ytdFile->time = SDL_GetTicks() / 1000;
			return true;
		}
	}
	std::unordered_map<uint32_t, uint32_t>::iterator it; //FOUND ARCH ->TEXTURES FILE
	it = data.TextureDictionary.find(hash);
	if (it != data.TextureDictionary.end())
	{
		//std::cout << "YTD Found " << std::endl;
		std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator iter;
		iter = data.YtdEntries.find(it->second);
		if (iter != data.YtdEntries.end())
		{
			std::cout << "YTD Found " << iter->second->Name << std::endl;
			auto& element = *(iter->second);
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);

			memstream stream(outputBuffer.data(), outputBuffer.size());

			YtdFile* file = new YtdFile(stream, hash);
			file->time = SDL_GetTicks() / 1000;
			ytdLoader.push_back(file);
			//TextureManager::LoadTexture(file->textures[0]);
			//delete file;

			return true;
		}
	}
}

bool GameWorld::LoadYDD(uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	for (auto& yddFile : yddLoader)
	{
		if (yddFile->Hash == hash) return true;
	}
	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
	it = data.YddEntries.find(hash);
	if (it != data.YddEntries.end())
	{
		std::cout << "YDD Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		//YddLoader test(stream, position, rotation, hash);
		yddLoader.emplace_back(new YddLoader(stream, position, rotation, scale, hash));

		return true;
	}
	else
	{
		//std::cout << "Element Not Found" << std::endl;
		return false;
	}
}

bool GameWorld::LoadYDR(uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	for (auto& ydrFile : ydrLoader)
	{
		if (ydrFile->Hash == hash) {
			ydrFile->time = SDL_GetTicks() / 1000;
			return true;
		}
	}
	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
	it = data.YdrEntries.find(hash);
	if (it != data.YdrEntries.end())
	{
		std::cout << "YDR Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);
		//printf(" SIZE BUFFER %d MB\n", outputBuffer.size() * sizeof(uint8_t)/1024/1024);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		YdrLoader *newYdr = new YdrLoader(stream, position, rotation, scale, hash, dynamicsWorld);
		newYdr->time = SDL_GetTicks() / 1000;
		ydrLoader.emplace_back(newYdr);

		return true;
	}
	else
	{
		//std::cout << "Element Not Found" << std::endl;
		return false;
	}
}

bool GameWorld::LoadYFT(uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	for (auto& yftFile : yftLoader)
	{
		if (yftFile->Hash == hash) {
			//yftFile->time = SDL_GetTicks() / 1000;
			return true;
		}
	}
	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
	it = data.YftEntries.find(hash);
	if (it != data.YftEntries.end())
	{
		std::cout << "YFT Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		yftLoader.emplace_back(new YftLoader(stream, position, rotation, scale, hash, dynamicsWorld));

		return true;
	}
	else
	{
		//std::cout << "Element Not Found" << std::endl;
		return false;
	}
}

bool GameWorld::LoadYBN(uint32_t hash)
{
	for (auto& ybnFile : ybnLoader)
	{
		if (ybnFile->Hash == hash) {
			ybnFile->time = SDL_GetTicks() / 1000;
			return true;
		}
	}
		std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
		it = data.YbnEntries.find(hash);
		if (it != data.YbnEntries.end())
		{
			std::cout << "YBN Found " << it->second->Name << std::endl;
			auto& element = *(it->second);
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);
			//printf(" SIZE BUFFER %d MB\n", outputBuffer.size() * sizeof(uint8_t) / 1024 / 1024);

			memstream stream(outputBuffer.data(), outputBuffer.size());
			//YbnLoader test(dynamicsWorld, stream, hash);
			YbnLoader *newYbn = new YbnLoader(dynamicsWorld, stream, hash);
			newYbn->time = SDL_GetTicks() / 1000;
			ybnLoader.emplace_back(newYbn);

			return true;
		}
		else
		{
			//std::cout << "Element Not Found" << std::endl;
			return false;
		}
}

void GameWorld::GetVisibleYmaps(glm::vec3 Position)
{
	SpaceGridCell& cell = spaceGrid.GetCell(Position);

	//DetectInWater(Position);

	for (auto& BoundsItem : cell.BoundsStoreItems)
	{
		LoadYBN(BoundsItem->Name);
	}

	for (auto& mapNode : cell.MapNodes)
	{
		LoadYmap(mapNode->Name, Position);
	}

	for (auto& Proxy : cell.CInteriorProxies)
	{
		LoadYBN(Proxy->Name);
		LoadYmap(Proxy->Parent, Position);
	}

	uint32_t curTime = SDL_GetTicks() / 1000;

	for (int i = 0; i < ybnLoader.size(); i++) {
		if ((curTime - ybnLoader[i]->time) > 5) {
			delete ybnLoader[i];
			ybnLoader.erase(ybnLoader.begin() + i);
		}
	}

	//CLEARING
	for (int i = 0; i < ymapLoader.size(); i++) {
		if ((curTime - ymapLoader[i]->time) > 5) {
			delete ymapLoader[i];
			ymapLoader.erase(ymapLoader.begin() + i);
		}
	}

	for (int i = 0; i < ydrLoader.size(); i++) {
		if ((curTime - ydrLoader[i]->time) > 5) {
			delete ydrLoader[i];
			ydrLoader.erase(ydrLoader.begin() + i);
		}
	}

	for (int i = 0; i < ytdLoader.size(); i++) {
		if ((curTime - ytdLoader[i]->time) > 5) {
			delete ytdLoader[i];
			ytdLoader.erase(ytdLoader.begin() + i);
		}
	}

	/*for (int i = 0; i < ymapLoader.size(); i++)
	{
		auto it = std::find_if(cell.MapNodes.begin(), cell.MapNodes.end(), [this , &i](MapDataStoreNode* node) -> bool { return this->ymapLoader[i]->Hash == node->Name; });
		if (it != cell.MapNodes.end()) {

		}
		else {
			//printf("DELETED!\n");

			for (auto& MapFile : ymapLoader[i]->CEntityDefs)
			{
				auto it2 = std::find_if(ydrLoader.begin(), ydrLoader.end(), [&MapFile](YdrLoader* loader) -> bool { return MapFile.archetypeName == loader->Hash; });
				if (it2 != ydrLoader.end()) {
					delete *it2;
					it2 = ydrLoader.erase(it2);
				}
				else {
					it2++;
				}

				auto it3 = std::find_if(yddLoader.begin(), yddLoader.end(), [&MapFile](YddLoader* loader) -> bool { return MapFile.archetypeName == loader->Hash; });
				if (it3 != yddLoader.end()) {
					delete *it3;
					it3 = yddLoader.erase(it3);
				}
				else {
					it3++;
				}
			}
			delete ymapLoader[i];
			ymapLoader.erase(ymapLoader.begin() + i);
		}
	}*/

	/*for (auto& BoundsItem : cell.BoundsStoreItems)
	{
		auto it = std::find_if(ybnLoader.begin(), ybnLoader.end(), [BoundsItem](YbnLoader* m) -> bool { return BoundsItem->Name == m->Hash; });
		if (it != ybnLoader.end()) {

		} else{
			LoadYBN(BoundsItem->Name);
		}
	}

	for (int i = 0; i < ybnLoader.size(); i++)
	{
		auto it = std::find_if(cell.BoundsStoreItems.begin(), cell.BoundsStoreItems.end(), [this, &i](BoundsStoreItem* item) -> bool { return this->ybnLoader[i]->Hash == item->Name; });
		if (it != cell.BoundsStoreItems.end()) {

		}
		else {
			//printf("NEED TO DELETE");
			delete ybnLoader[i];
			ybnLoader.erase(ybnLoader.begin() + i);
		}
	}*/
}

void GameWorld::createPedestrian()
{
	Player *newPlayer = new Player(glm::vec3(0,20,0), dynamicsWorld);
	pedestrians.push_back(newPlayer);
}

void GameWorld::createVehicle()
{
	Vehicle *newVehicle = new Vehicle(glm::vec3(-20,0,0), dynamicsWorld);
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
		vehicle->PhysicsTick();
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
			dynamicsWorld->removeCollisionObject(pedestrians[i]->getPhysCharacter()->getGhostObject());
			dynamicsWorld->removeCharacter(pedestrians[i]->getPhysCharacter());
			delete pedestrians[i];
			pedestrians.erase(pedestrians.begin() + i);
		}
	}

	int MaximumAvailablePeds = 20 - pedestrians.size(); //HARDCODED
	if (cameraPosition.z < 100.0f) {
		for (int i = 0; i < MaximumAvailablePeds; i++) {
			float xRandom = RandomFloat(cameraPosition.x - radiusTraffic, cameraPosition.x + radiusTraffic);
			float yRandom = RandomFloat(cameraPosition.y - radiusTraffic, cameraPosition.y + radiusTraffic);
			Player *newPlayer = new Player(glm::vec3(xRandom, yRandom, cameraPosition.z + 5.0f), dynamicsWorld);
			pedestrians.push_back(newPlayer);
		}
	}
	//CARS
	for (int i = 0; i < vehicles.size(); i++) {
		glm::vec3 vehiclePosition(vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getX(), vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getY(), vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getZ());
		if (glm::distance(cameraPosition, vehiclePosition) >= 100.0f) {
			dynamicsWorld->removeVehicle((vehicles[i]->m_vehicle));
			dynamicsWorld->removeRigidBody((vehicles[i]->m_carChassis));
			vehicles.erase(vehicles.begin() + i);
		}
	}

	int MaximumAvailableVehicles = 10 - vehicles.size(); //HARDCODED
	if (cameraPosition.z < 100.0f) {
		for (int i = 0; i < MaximumAvailableVehicles; i++) {
			float xRandom = RandomFloat(cameraPosition.x - radiusTraffic, cameraPosition.x + radiusTraffic);
			float yRandom = RandomFloat(cameraPosition.y - radiusTraffic, cameraPosition.y + radiusTraffic);
			Vehicle* newVehicle= new Vehicle(glm::vec3(xRandom, yRandom, cameraPosition.z + 5.0f), dynamicsWorld);
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
		glm::vec3 VehiclePosition(vehicle->m_carChassis->getWorldTransform().getOrigin().getX(), vehicle->m_carChassis->getWorldTransform().getOrigin().getY(), vehicle->m_carChassis->getWorldTransform().getOrigin().getZ());
		float vd = glm::length(PlayerPosition - VehiclePosition);
		if (vd < d) {
			d = vd;
			nearestVehicle = vehicle;
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



void GameWorld::update(float delta_time)
{
	Update();
	dynamicsWorld->stepSimulation(delta_time, 4, 1.0f / 30.0f);
}



void GameWorld::TestFunction(glm::vec3 Position)
{
	ClearTestFunction();
}

bool GameWorld::DetectInWater(glm::vec3 Position) {
	for (auto& waterQuad : data.WaterQuads)
	{
		if (waterQuad.minX < Position.x && waterQuad.maxX > Position.x && waterQuad.minY < Position.y && waterQuad.maxY > Position.y)
		{
			//printf("WE ARE IN WATER\n");
			return true;
		}
	}
	return false;
}

void GameWorld::ClearTestFunction()
{
	//int test = ybnLoader.size();
	for (int i = 0; i < ybnLoader.size(); i++) {
			delete ybnLoader[i];
			
	}
	ybnLoader.erase(ybnLoader.begin(), ybnLoader.end());

	//CLEARING
	/*for (int i = 0; i < ymapLoader.size(); i++) {
			delete ymapLoader[i];
			ymapLoader.erase(ymapLoader.begin() + i);
	}

	for (int i = 0; i < ydrLoader.size(); i++) {
		delete ydrLoader[i];
		ydrLoader.erase(ydrLoader.begin() + i);
	}*/
}
