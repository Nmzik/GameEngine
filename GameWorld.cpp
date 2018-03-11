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

	player = new Player(glm::vec3(0, 0, 20), dynamicsWorld);

	gameHour = 10;
	gameMinute = 0;

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe |
		btIDebugDraw::DBG_DrawConstraints |
		btIDebugDraw::DBG_DrawConstraintLimits);
	dynamicsWorld->setDebugDrawer(&debug);

	//LoadYDR(3186271474, glm::vec3(), glm::quat(), glm::vec3());

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

void GameWorld::LoadYmap(Shader* shader, uint32_t hash, glm::vec3 cameraPosition)
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
				if (map->CEntityDefs[i].lodDist < 0 || map->CEntityDefs[i].childLodDist < 0) {
					//printf("HERE");
				}
				else {
					bool IsVisible = glm::distance(cameraPosition, map->CEntityDefs[i].position) <= map->CEntityDefs[i].lodDist;
					bool childrenVisible = (glm::distance(cameraPosition, map->CEntityDefs[i].position) <= map->CEntityDefs[i].childLodDist) && (map->CEntityDefs[i].numChildren > 0);
					if (IsVisible && !childrenVisible) {
						LoadYTD(map->CEntityDefs[i].archetypeName);
							if (!LoadYDR(shader, map->CEntityDefs[i].archetypeName, map->CEntityDefs[i].position, glm::quat(-map->CEntityDefs[i].rotation.w, map->CEntityDefs[i].rotation.x, map->CEntityDefs[i].rotation.y, map->CEntityDefs[i].rotation.z), glm::vec3(map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleZ)))
								if (!LoadYDD(shader, map->CEntityDefs[i].archetypeName, map->CEntityDefs[i].position, glm::quat(-map->CEntityDefs[i].rotation.w, map->CEntityDefs[i].rotation.x, map->CEntityDefs[i].rotation.y, map->CEntityDefs[i].rotation.z), glm::vec3(map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleZ)))
									LoadYFT(shader, map->CEntityDefs[i].archetypeName, map->CEntityDefs[i].position, glm::quat(-map->CEntityDefs[i].rotation.w, map->CEntityDefs[i].rotation.x, map->CEntityDefs[i].rotation.y, map->CEntityDefs[i].rotation.z), glm::vec3(map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleZ));
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
	std::unordered_map<uint32_t, CBaseArchetypeDef>::iterator it; //FOUND ARCH ->TEXTURES FILE
	it = data.TextureDictionary.find(hash);
	if (it != data.TextureDictionary.end())
	{
		for (auto& ytdFile : ytdLoader)
		{
			if (ytdFile->Hash == it->second.textureDictionary) {
				ytdFile->time = SDL_GetTicks() / 1000;
				return true;
			}
		}
		//std::cout << "YTD Found " << std::endl;
		std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator iter;
		iter = data.YtdEntries.find(it->second.textureDictionary);
		if (iter != data.YtdEntries.end())
		{
			std::cout << "YTD Found " << iter->second->Name << std::endl;
			auto& element = *(iter->second);
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);

			memstream stream(outputBuffer.data(), outputBuffer.size());

			YtdFile* file = new YtdFile(stream, it->second.textureDictionary);
			file->time = SDL_GetTicks() / 1000;
			ytdLoader.push_back(file);
			//TextureManager::LoadTexture(file->textures[0]);
			//delete file;

			return true;
		}
	}
}

bool GameWorld::LoadYDD(Shader* shader, uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	for (auto& yddFile : yddLoader)
	{
		if (yddFile->Hash == hash) {
			yddFile->time = SDL_GetTicks() / 1000;

			if (yddFile->YdrFile != nullptr) {
				auto modelpos = yddFile->GetMat4();
				shader->setMat4(3, modelpos);
				yddFile->Draw();
			}
			return true;
		}
	}

	std::unordered_map<uint32_t, CBaseArchetypeDef>::iterator iter;
	iter = data.TextureDictionary.find(hash);
	if (iter != data.TextureDictionary.end())
	{

		std::unordered_map<uint32_t, RpfResourceFileEntry*>::iterator it;
		it = data.YddEntries.find(iter->second.drawableDictionary);
		if (it != data.YddEntries.end())
		{
			std::cout << "YDD Found " << it->second->Name << std::endl;
			auto& element = *(it->second);
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);

			memstream stream(outputBuffer.data(), outputBuffer.size());
			YddLoader* newYdd = new YddLoader(stream, position, rotation, scale, hash, dynamicsWorld);
			newYdd->time = SDL_GetTicks() / 1000;
			yddLoader.emplace_back(newYdd);

			return true;
		}
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
		YddLoader* newYdd = new YddLoader(stream, position, rotation, scale, hash, dynamicsWorld);
		newYdd->time = SDL_GetTicks() / 1000;
		yddLoader.emplace_back(newYdd);

		return true;
	}
	else
	{
		//std::cout << "Element Not Found" << std::endl;
		return false;
	}
}

bool GameWorld::LoadYDR(Shader* shader, uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	for (auto& ydrFile : ydrLoader)
	{
		if (ydrFile->Hash == hash) {
			ydrFile->time = SDL_GetTicks() / 1000;

			auto modelpos = ydrFile->GetMat4();
			shader->setMat4(3, modelpos);
			ydrFile->Draw();

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

bool GameWorld::LoadYFT(Shader* shader, uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	for (auto& yftFile : yftLoader)
	{
		if (yftFile->Hash == hash) {
			yftFile->time = SDL_GetTicks() / 1000;

			auto modelpos = yftFile->GetMat4();
			shader->setMat4(3, modelpos);
			yftFile->Draw();

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
		YftLoader *newYft = new YftLoader(stream, position, rotation, scale, hash, dynamicsWorld);
		newYft->time = SDL_GetTicks() / 1000;
		yftLoader.emplace_back(newYft);

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

		memstream stream(outputBuffer.data(), outputBuffer.size());

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

void GameWorld::GetVisibleYmaps(Shader* shader, glm::vec3 Position)
{
	SpaceGridCell& cell = spaceGrid.GetCell(Position);

	//DetectInWater(Position);

	for (auto& BoundsItem : cell.BoundsStoreItems)
	{
		LoadYBN(BoundsItem->Name);
	}

	for (auto& mapNode : cell.MapNodes)
	{
		LoadYmap(shader, mapNode->Name, Position);
	}

	/*for (auto& Proxy : cell.CInteriorProxies)
	{
		LoadYBN(Proxy->Name);
		LoadYmap(Proxy->Parent, Position);
	}*/

	uint32_t curTime = SDL_GetTicks() / 1000;

	for (std::vector<YbnLoader*>::iterator it = ybnLoader.begin(); it != ybnLoader.end(); /*increment in body*/)
	{
		if (curTime - (*it)->time > UnloadTime)
		{
			delete *it;
			it = ybnLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (std::vector<YmapLoader*>::iterator it = ymapLoader.begin(); it != ymapLoader.end(); /*increment in body*/)
	{
		if (curTime - (*it)->time > UnloadTime)
		{
			delete *it;
			it = ymapLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (std::vector<YdrLoader*>::iterator it = ydrLoader.begin(); it != ydrLoader.end(); /*increment in body*/)
	{
		if (curTime - (*it)->time > UnloadTime)
		{
			//printf("DELETED %ld", (*it)->Hash);
			delete *it;
			it = ydrLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (std::vector<YddLoader*>::iterator it = yddLoader.begin(); it != yddLoader.end(); /*increment in body*/)
	{
		if (curTime - (*it)->time > UnloadTime)
		{
			//printf("DELETED %ld", (*it)->Hash);
			delete *it;
			it = yddLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (std::vector<YftLoader*>::iterator it = yftLoader.begin(); it != yftLoader.end(); /*increment in body*/)
	{
		if (curTime - (*it)->time > UnloadTime)
		{
			delete *it;
			it = yftLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (std::vector<YtdFile*>::iterator it = ytdLoader.begin(); it != ytdLoader.end(); /*increment in body*/)
	{
		if (curTime - (*it)->time > UnloadTime)
		{
			delete *it;
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
	Player *newPlayer = new Player(glm::vec3(0, 20, 0), dynamicsWorld);
	pedestrians.push_back(newPlayer);
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
	ybnLoader.resize(300);
	for (int i = 0; i < 100; i++)
	{
		LoadYBN(2081927167);
	}

	for (std::vector<YbnLoader*>::iterator it = ybnLoader.begin(); it != ybnLoader.end(); /*increment in body*/)
	{
		delete *it;
		it = ybnLoader.erase(it);
	}
}
