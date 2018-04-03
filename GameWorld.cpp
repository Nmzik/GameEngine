#include "GameWorld.h"

#define UnloadTime 2

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

	//_ResourceManager = new ResourceManager(this);

	for (int i = 0; i < cacheFile.AllMapNodes.size(); i++)
	{
		spaceGrid.AddMapNode(&cacheFile.AllMapNodes[i], i);
	}

	for (int i = 0; i < cacheFile.AllBoundsStoreItems.size(); i++)
	{
		spaceGrid.AddBoundsItem(&cacheFile.AllBoundsStoreItems[i], i);
	}

	for (int i = 0; i < cacheFile.AllCInteriorProxies.size(); i++)
	{
		spaceGrid.AddCInteriorProxy(&cacheFile.AllCInteriorProxies[i], i);
	}

	ydrLoader.reserve(500);
	yddLoader.reserve(500);;
	yftLoader.reserve(500);
	ytdLoader.reserve(500);
	//yndLoader.reserve(500);
	ybnLoader.reserve(500);
	ytypLoader.reserve(500);
	ymapLoader.reserve(500);

	/*{
			//std::cout << "YTD Found " << std::endl;
			std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator iter;
			iter = data.YtdEntries.find(3523992128);
			if (iter != data.YtdEntries.end())
			{
				std::cout << "YTD Found " << iter->second->Name << std::endl;
				auto& element = *(iter->second);
				std::vector<uint8_t> outputBuffer;
				data.ExtractFileResource(element, outputBuffer);

				memstream stream(outputBuffer.data(), outputBuffer.size());

				YtdFile* file = new YtdFile(stream, 3523992128);
			}
	}*/
	/*LoadYTD(3523992128); //water.ytd
	for (auto& WaterQuad : data.WaterQuads)
	{
		Water water(WaterQuad);
		WaterMeshes.push_back(water);
	}*/

	gameHour = 10;
	gameMinute = 0;

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe |
		btIDebugDraw::DBG_DrawConstraints |
		btIDebugDraw::DBG_DrawConstraintLimits);
	dynamicsWorld->setDebugDrawer(&debug);
	//LoadYDR(3186271474, glm::vec3(), glm::quat(), glm::vec3());

	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator itPlayer = data.YddEntries.find(4096714883);
	if (itPlayer != data.YddEntries.end())
	{
		std::cout << "PLAYER Found " << itPlayer->second->Name << std::endl;
		auto& element = *(itPlayer->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		YddLoader* playerYDD = new YddLoader(stream, dynamicsWorld);

		player = new Player(glm::vec3(0, 0, 20), playerYDD, dynamicsWorld);
	}

	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
	it = data.YddEntries.find(2640562617);
	if (it != data.YddEntries.end())
	{
		std::cout << "SKYDOME Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		skydome = new YddLoader(stream, dynamicsWorld);
		std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator iter;
		iter = data.YtdEntries.find(2640562617);
		if (iter != data.YtdEntries.end())
		{
			std::cout << "YTD Found " << iter->second->Name << std::endl;
			auto& element = *(iter->second);
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);

			memstream stream(outputBuffer.data(), outputBuffer.size());

			YtdFile* file = new YtdFile(stream);
		}

		//skydome->YdrFiles[0]->meshes[0]->material->diffuseTextureID = TextureManager::GetTexture(1064311147);
		return;
	}



	//ClearTestFunction();
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


}

GameWorld::~GameWorld()
{
	delete dynamicsWorld;
}

void GameWorld::LoadYmap(Shader* shader, uint32_t hash, Camera* camera)
{
	auto it = ymapLoader.find(hash);
	if (it != ymapLoader.end()) {
		YmapLoader* map = it->second;
		//printf("FOUND SAME\n");
		//FOUND
		map->time = SDL_GetTicks() / 1000;
		if (!(map->_CMapData.flags & 1) > 0) { //DONT LOAD SCRIPTED MAPS
			for (int i = 0; i < map->CEntityDefs.size(); i++)
			{
				/*bool isreflproxy = false;
				switch (map->CEntityDefs[i].flags)
				{
				case 135790592: //001000000110000000000000000000    prewater proxy (golf course)
				case 135790593: //001000000110000000000000000001    water refl proxy? (mike house)
				case 672661504: //101000000110000000000000000000    vb_ca_prop_tree_reflprox_2
				case 536870912: //100000000000000000000000000000    vb_05_emissive_mirroronly
				case 35127296:  //000010000110000000000000000000    tunnel refl proxy?
				case 39321602:  //000010010110000000000000000010    mlo reflection?
					isreflproxy = true; break;
				}
				if (isreflproxy) {
					continue;
				}*/
				if (map->CEntityDefs[i].lodDist < 0 || map->CEntityDefs[i].childLodDist < 0) {
					//printf("HERE");
				}
				else {
					bool IsVisible = glm::length(camera->Position - map->CEntityDefs[i].position) <= map->CEntityDefs[i].lodDist * LODMultiplier;
					bool childrenVisible = (glm::length(camera->Position - map->CEntityDefs[i].position) <= map->CEntityDefs[i].childLodDist * LODMultiplier) && (map->CEntityDefs[i].numChildren > 0);
					if (IsVisible && !childrenVisible) {
						std::unordered_map<uint32_t, CBaseArchetypeDef>::iterator it = data.CBaseArchetypeDefs.find(map->CEntityDefs[i].archetypeName);
						if (it != data.CBaseArchetypeDefs.end())
						{
							if (camera->intersects(it->second.bsCentre + map->CEntityDefs[i].position - camera->Position, it->second.bsRadius)) {
								LoadYTD(it->second.textureDictionary);
								if (it->second.assetType == ASSET_TYPE_DRAWABLE)
									LoadYDR(shader, map->CEntityDefs[i].archetypeName, map->ModelMatrices[i]);
								if (it->second.assetType == ASSET_TYPE_DRAWABLEDICTIONARY)
									LoadYDD(shader, map->CEntityDefs[i].archetypeName, it->second.drawableDictionary, map->ModelMatrices[i]);
								if (it->second.assetType == ASSET_TYPE_FRAGMENT)
									LoadYFT(shader, map->CEntityDefs[i].archetypeName, map->ModelMatrices[i]);
							}
						}
						else {
							std::unordered_map<uint32_t, CTimeArchetypeDef>::iterator it = data.CTimeArchetypeDefs.find(map->CEntityDefs[i].archetypeName);
							if (it != data.CTimeArchetypeDefs.end())
							{
								//TIME FLAGS FOUND
								if ((it->second._TimeArchetypeDef.timeFlags >> gameHour) & 1) {
									LoadYTD(it->second._BaseArchetypeDef.textureDictionary);
									if (it->second._BaseArchetypeDef.assetType == ASSET_TYPE_DRAWABLE)
										LoadYDR(shader, map->CEntityDefs[i].archetypeName, map->ModelMatrices[i]);
									if (it->second._BaseArchetypeDef.assetType == ASSET_TYPE_DRAWABLEDICTIONARY)
										LoadYDD(shader, map->CEntityDefs[i].archetypeName, it->second._BaseArchetypeDef.drawableDictionary, map->ModelMatrices[i]);
									if (it->second._BaseArchetypeDef.assetType == ASSET_TYPE_FRAGMENT)
										LoadYFT(shader, map->CEntityDefs[i].archetypeName, map->ModelMatrices[i]);
								}
							}
							//else {
								//printf("NOT FOUND");
							//}
						}
						//LoadDrawable(shader, map->CEntityDefs[i].archetypeName, map->ModelMatrices[i]);
					}
				}
			}
		}
	}
	else {
		std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it = data.YmapEntries.find(hash);
		if (it != data.YmapEntries.end())
		{
			//std::cout << "YMAP Found" << std::endl;
			auto& element = *(it->second);

			//CAN BE AN ERROR HERE - NOT FULLY IMPLEMENTED!
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);

			memstream stream(outputBuffer.data(), outputBuffer.size());

			YmapLoader *ymap = new YmapLoader(stream);
			ymap->time = SDL_GetTicks() / 1000;
			ymapLoader[hash] = ymap;
		}
	}
}

bool GameWorld::LoadYTYP(uint32_t hash)
{
	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it = data.YtypEntries.find(hash);
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
}

void GameWorld::LoadYTD(uint32_t hash)
{
	std::unordered_map<uint32_t, YtdFile*>::iterator it = ytdLoader.find(hash);
	if (it != ytdLoader.end())
	{
		it->second->time = SDL_GetTicks() / 1000;

		return;
	}

	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator iter = data.YtdEntries.find(hash);
	if (iter != data.YtdEntries.end())
	{
		std::cout << "YTD Found " << iter->second->Name << std::endl;
		auto& element = *(iter->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		YtdFile* file = new YtdFile(stream);
		file->time = SDL_GetTicks() / 1000;
		ytdLoader[hash] = file;
		//TextureManager::LoadTexture(file->textures[0]);
		//delete file;
	}
}

void GameWorld::LoadYDR(Shader * shader, uint32_t hash, glm::mat4 & matrix)
{
	std::unordered_map<uint32_t, YdrLoader*>::iterator iter = ydrLoader.find(hash);
	if (iter != ydrLoader.end())
	{
		iter->second->time = SDL_GetTicks() / 1000;

		shader->setMat4(3, matrix);
		iter->second->Draw();

		return;
	}

	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it = data.YdrEntries.find(hash);
	if (it != data.YdrEntries.end())
	{
		std::cout << "YDR Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);
		//printf(" SIZE BUFFER %d MB\n", outputBuffer.size() * sizeof(uint8_t)/1024/1024);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		YdrLoader *newYdr = new YdrLoader(stream, dynamicsWorld);
		newYdr->time = SDL_GetTicks() / 1000;
		ydrLoader[hash] = newYdr;

		return;
	}
}

void GameWorld::LoadYDD(Shader * shader, uint32_t hash, uint32_t DrawableDictionaryHash, glm::mat4 & matrix)
{
	std::unordered_map<uint32_t, YddLoader*>::iterator iter = yddLoader.find(DrawableDictionaryHash);
	if (iter != yddLoader.end())
	{
		std::unordered_map<uint32_t, YdrLoader*>::iterator iter2 = iter->second->YdrFiles.find(hash);
		if (iter2 != iter->second->YdrFiles.end())
		{
			shader->setMat4(3, matrix);
			iter2->second->Draw();

			iter->second->time = SDL_GetTicks() / 1000;
			return;
		}
	}

	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it = data.YddEntries.find(DrawableDictionaryHash);
	if (it != data.YddEntries.end())
	{
		std::cout << "YDD Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		YddLoader* newYdd = new YddLoader(stream, dynamicsWorld);
		newYdd->time = SDL_GetTicks() / 1000;
		yddLoader[DrawableDictionaryHash] = newYdd;

		return;
	}

}

void GameWorld::LoadYFT(Shader * shader, uint32_t hash, glm::mat4 & matrix)
{
	std::unordered_map<uint32_t, YftLoader*>::iterator iter = yftLoader.find(hash);
	if (iter != yftLoader.end())
	{
		iter->second->time = SDL_GetTicks() / 1000;

		shader->setMat4(3, matrix);
		iter->second->Draw();

		return;
	}

	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it = data.YftEntries.find(hash);
	if (it != data.YftEntries.end())
	{
		std::cout << "YFT Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		YftLoader *newYft = new YftLoader(stream, dynamicsWorld);
		newYft->time = SDL_GetTicks() / 1000;
		yftLoader[hash] = newYft;

		return;
	}

}

void GameWorld::LoadYBN(uint32_t hash)
{
	std::unordered_map<uint32_t, YbnLoader*>::iterator iter = ybnLoader.find(hash);
	if (iter != ybnLoader.end())
	{
		iter->second->time = SDL_GetTicks() / 1000;
		return;
	}
	std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it = data.YbnEntries.find(hash);
	if (it != data.YbnEntries.end())
	{
		std::cout << "YBN Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		YbnLoader *newYbn = new YbnLoader(dynamicsWorld, stream);
		newYbn->time = SDL_GetTicks() / 1000;
		ybnLoader[hash] = newYbn;

		return;
	}
}

void GameWorld::GetVisibleYmaps(Shader* shader, Camera* camera)
{
	SpaceGridCell& cell = spaceGrid.GetCell(camera->Position);

	//DetectInWater(Position);

	for (auto& BoundsItem : cell.BoundsStoreItems)
	{
		LoadYBN(cacheFile.AllBoundsStoreItems[BoundsItem].Name);
	}

	for (auto& mapNode : cell.MapNodes)
	{
		LoadYmap(shader, cacheFile.AllMapNodes[mapNode].Name, camera);
	}

	/*for (auto& Proxy : cell.CInteriorProxies)
	{
		LoadYmap(shader, Proxy->Parent, camera);
		for (auto& ymapFile : ymapLoader)
		{
			if (ymapFile->Hash == Proxy->Parent) {
				for (int i = 0; i < ymapFile->CMloInstanceDefs.size(); i++)
				{
					std::unordered_map<uint32_t, std::vector<CEntityDef>>::iterator it = data.MloDictionary.find(ymapFile->CMloInstanceDefs[i].CEntityDef.archetypeName);
					if (it != data.MloDictionary.end())
					{
						for (auto& EntityDef : it->second)
						{
							LoadYTD(EntityDef.archetypeName);
							glm::mat4 matrix = glm::translate(glm::mat4(), ymapFile->CMloInstanceDefs[i].CEntityDef.position + EntityDef.position) * glm::toMat4(glm::quat(-ymapFile->CMloInstanceDefs[i].CEntityDef.rotation.w, ymapFile->CMloInstanceDefs[i].CEntityDef.rotation.x, ymapFile->CMloInstanceDefs[i].CEntityDef.rotation.y, ymapFile->CMloInstanceDefs[i].CEntityDef.rotation.z) * glm::quat(-EntityDef.rotation.w, EntityDef.rotation.x, EntityDef.rotation.y, EntityDef.rotation.z)) * glm::scale(glm::mat4(), glm::vec3(EntityDef.scaleXY, EntityDef.scaleXY, EntityDef.scaleZ));
							LoadDrawable(shader, EntityDef.archetypeName, matrix);
						}

					}
				}
			}
		}

		//LoadYBN(Proxy->Name);
		//LoadYmap(Proxy->Parent, Position);
	}*/

	//printf("CULLED :%d\n", culled);
	//culled = 0;

	uint32_t curTime = SDL_GetTicks() / 1000;

	for (auto it = ybnLoader.begin(); it != ybnLoader.end();)
	{
		if (curTime - (it->second)->time > UnloadTime)
		{
			delete it->second;
			it = ybnLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = ymapLoader.begin(); it != ymapLoader.end();)
	{
		if (curTime - (it->second)->time > UnloadTime)
		{
			delete it->second;
			it = ymapLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = ydrLoader.begin(); it != ydrLoader.end();)
	{
		if (curTime - (it->second)->time > UnloadTime)
		{
			delete it->second;
			it = ydrLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = yddLoader.begin(); it != yddLoader.end();)
	{
		if (curTime - (it->second)->time > UnloadTime)
		{
			delete it->second;
			it = yddLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = yftLoader.begin(); it != yftLoader.end();)
	{
		if (curTime - (it->second)->time > UnloadTime)
		{
			delete it->second;
			it = yftLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = ytdLoader.begin(); it != ytdLoader.end();)
	{
		if (curTime - (it->second)->time > UnloadTime)
		{
			delete it->second;
			it = ytdLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void GameWorld::createPedestrian()
{
	//Player *newPlayer = new Player(glm::vec3(0, 20, 0), dynamicsWorld);
	//pedestrians.push_back(newPlayer);
}

void GameWorld::createVehicle()
{
	Vehicle *newVehicle = new Vehicle(glm::vec3(-20, 0, 0), dynamicsWorld);
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
			//Player *newPlayer = new Player(glm::vec3(xRandom, yRandom, cameraPosition.z + 5.0f), dynamicsWorld);
			//pedestrians.push_back(newPlayer);
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
			Vehicle* newVehicle = new Vehicle(glm::vec3(xRandom, yRandom, cameraPosition.z + 5.0f), dynamicsWorld);
			vehicles.push_back(newVehicle);
		}
	}
}

Vehicle* GameWorld::FindNearestVehicle()
{
	float d = 15.0f;

	Vehicle* nearestVehicle = nullptr;

	for (auto& vehicle : vehicles)
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
	dynamicsWorld->stepSimulation(delta_time);
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
	/*ybnLoader.resize(300);
	for (int i = 0; i < 100; i++)
	{
		LoadYBN(2081927167);
	}

	for (std::vector<YbnLoader*>::iterator it = ybnLoader.begin(); it != ybnLoader.end();)
	{
		delete *it;
		it = ybnLoader.erase(it);
	}*/
}
