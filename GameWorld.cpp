#include "GameWorld.h"

#define UnloadTime 4000

GameWorld::GameWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, -9.8f));

	//Default texture
	TextureManager::LoadTexture(0, TextureManager::loadTexture("blank.jpg"));
	//////////////////////////////////////////////////////////////////////////////

	_ResourceManager = new ResourceManager(this);

	//Nodes
	for (int x = 0; x < nodeGrid.CellCountX; x++)
	{
		for (int y = 0; y < nodeGrid.CellCountY; y++)
		{
			std::string filename = "nodes" + std::to_string(nodeGrid.cells[x * nodeGrid.CellCountX + y]->ID) + ".ynd";
			uint32_t fnhash = GenHash(filename);

			auto it = data.YndEntries.find(fnhash);
			if (it != data.YndEntries.end()) {

				auto& element = *(it->second);
				std::vector<uint8_t> outputBuffer;
				data.ExtractFileResource(element, outputBuffer);

				memstream stream(outputBuffer.data(), outputBuffer.size());

				YndLoader* ynd = new YndLoader(stream);
				nodeGrid.cells[x * nodeGrid.CellCountX + y]->ynd = ynd;
			}
		}
	}

	for (int i = 0; i < cacheFile.AllMapNodes.size(); i++)
	{
		if (cacheFile.AllMapNodes[i].Unk1 == 1) //NOT SURE
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

	for (auto& vehicle : data.Vehicles)
	{
		vehiclesPool[vehicle.Hash] = vehicle;
	}

	ydrLoader.reserve(500);
	yddLoader.reserve(500);;
	yftLoader.reserve(500);
	ytdLoader.reserve(500);
	//yndLoader.reserve(500);
	ybnLoader.reserve(500);
	ytypLoader.reserve(500);
	ymapLoader.reserve(500);

	pedestrians.reserve(20);

	//RenderList
	renderList.reserve(2000);
	/*{
			//std::cout << "YTD Found " << std::endl;
			auto iter;
			iter = data.YtdEntries.find(3523992128);
			if (iter != data.YtdEntries.end())
			{
				std::cout << "YTD Found " << iter->second->Name << std::endl;
				auto& element = *(iter->second);
				std::vector<uint8_t> outputBuffer;
				data.ExtractFileResource(element, outputBuffer);

				memstream stream(outputBuffer.data(), outputBuffer.size());

				YtdLoader* file = new YtdLoader(stream, 3523992128);
			}
	}*/
	//*LoadYTD(3523992128); //water.ytd
	for (auto& WaterQuad : data.WaterQuads)
	{
		Water water(WaterQuad);
		WaterMeshes.push_back(water);
	}

	gameHour = 10;
	gameMinute = 0;

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(&debug);
	//LoadYDR(3186271474, glm::vec3(), glm::quat(), glm::vec3());

	MeshManager::Initialize();
	TextureManager::Initialize();

	auto itPlayer = data.YddEntries.find(4096714883);
	if (itPlayer != data.YddEntries.end())
	{
		std::cout << "PLAYER Found " << itPlayer->second->Name << std::endl;
		auto& element = *(itPlayer->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		playerYDD = new YddLoader(stream, element.SystemSize, dynamicsWorld);

		player[0].Init(glm::vec3(2137, 3656, 100), playerYDD, dynamicsWorld);
		player[0].getPhysCharacter()->setGravity(btVector3(0, 0, 0));
		player[1].Init(glm::vec3(9.66, -1184.98, 75.74), playerYDD, dynamicsWorld);
		player[1].getPhysCharacter()->setGravity(btVector3(0, 0, 0));
		player[2].Init(glm::vec3(1983.69f, 3825.26, 66.38), playerYDD, dynamicsWorld);
		player[2].getPhysCharacter()->setGravity(btVector3(0, 0, 0));
	}

	auto it = data.YddEntries.find(2640562617);
	if (it != data.YddEntries.end())
	{
		std::cout << "SKYDOME Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());
		skydome = new YddLoader(stream, element.SystemSize, dynamicsWorld);
		auto iter = data.YtdEntries.find(2640562617);
		if (iter != data.YtdEntries.end())
		{
			std::cout << "YTD Found " << iter->second->Name << std::endl;
			auto& element = *(iter->second);
			std::vector<uint8_t> outputBuffer;
			data.ExtractFileResource(element, outputBuffer);

			memstream stream(outputBuffer.data(), outputBuffer.size());

			YtdLoader* file = new YtdLoader(stream, element.SystemSize);
		}
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

float RandomFloat(float min, float max) {
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

void GameWorld::LoadYmap(uint32_t hash, Camera* camera)
{
	YmapLoader *map = GetYmap(hash);
	if (map->Loaded) {
		for (auto& object : map->Objects)
		{
			float Dist = glm::length(camera->Position - object.position);
			bool IsVisible = Dist <= object.CEntity.lodDist * LODMultiplier;
			bool childrenVisible = (Dist <= object.CEntity.childLodDist * LODMultiplier) && (object.CEntity.numChildren > 0);
			if (IsVisible && !childrenVisible) {
				if (object.type == 0) {
					std::unordered_map<uint32_t, CBaseArchetypeDef>::iterator it = data.CBaseArchetypeDefs.find(object.CEntity.archetypeName);
					if (it != data.CBaseArchetypeDefs.end())
					{
						object.Archetype._BaseArchetypeDef = it->second;

						//if (object.CEntity.lodDist <= 0) object.CEntity.lodDist = 30.0f;
						//if (object.CEntity.childLodDist <= 0) object.CEntity.childLodDist = 30.0f;

						object.type = 1;
					}
					else {
						std::unordered_map<uint32_t, CTimeArchetypeDef>::iterator it = data.CTimeArchetypeDefs.find(object.CEntity.archetypeName);
						if (it != data.CTimeArchetypeDefs.end())
						{
							object.Archetype = it->second;

							//if (object.CEntity.lodDist <= 0) object.CEntity.lodDist = it->second._BaseArchetypeDef.lodDist;
							//if (object.CEntity.childLodDist <= 0) object.CEntity.childLodDist = it->second._BaseArchetypeDef.lodDist;

							object.type = 2;
						}
					}
				}
				else {

					if ((object.Archetype._BaseArchetypeDef.flags & 2048) > 0)
					{
						//if (!renderProxies) continue;
						continue;
					}

					if (object.type == 2) {
						if ((object.Archetype._TimeArchetypeDef.timeFlags >> gameHour) & 1)
						{
							continue;
						}
					}

					switch (object.Archetype._BaseArchetypeDef.assetType)
					{
						case ASSET_TYPE_DRAWABLE: {
							YdrLoader* file = GetYdr(object.CEntity.archetypeName, object.Archetype._BaseArchetypeDef.textureDictionary);
							if (file->Loaded) {
								if (camera->intersects(object.Archetype._BaseArchetypeDef.bsCentre + object.CEntity.position, object.Archetype._BaseArchetypeDef.bsRadius * std::max(object.CEntity.scaleXY, object.CEntity.scaleZ))) {
									renderList.emplace_back(file, object.getMatrix());
								}
							}
							break;
						}
						case ASSET_TYPE_DRAWABLEDICTIONARY: {
							YddLoader* file = GetYdd(object.Archetype._BaseArchetypeDef.drawableDictionary, object.Archetype._BaseArchetypeDef.textureDictionary);
							if (file->Loaded) {
								std::unordered_map<uint32_t, YdrLoader*>::iterator iter2 = file->YdrFiles.find(object.CEntity.archetypeName);
								if (iter2 != file->YdrFiles.end())
								{
									if (camera->intersects(object.Archetype._BaseArchetypeDef.bsCentre + object.CEntity.position, object.Archetype._BaseArchetypeDef.bsRadius * std::max(object.CEntity.scaleXY, object.CEntity.scaleZ))) {
										renderList.emplace_back(iter2->second, object.getMatrix());
									}
								}
							}
							break;
						}
						case ASSET_TYPE_FRAGMENT: {
							YftLoader* file = GetYft(object.CEntity.archetypeName, object.Archetype._BaseArchetypeDef.textureDictionary);
							if (file->Loaded) {
								if (camera->intersects(object.Archetype._BaseArchetypeDef.bsCentre + object.CEntity.position, object.Archetype._BaseArchetypeDef.bsRadius * std::max(object.CEntity.scaleXY, object.CEntity.scaleZ))) {
									renderList.emplace_back(file->YdrFile, object.getMatrix());
								}
							}
							break;
						}
					}

				}
			}
		}
	}
	/*if (map->CMloInstanceDefs.size() > 0) {
		for (int i = 0; i < map->CMloInstanceDefs.size(); i++)
		{
			auto it = data.MloDictionary.find(map->CMloInstanceDefs[i].CEntityDef.archetypeName);
			if (it != data.MloDictionary.end())
			{
				for (auto& EntityDef : it->second)
				{
					glm::vec4 rotmultiply = EntityDef.rotation * map->CMloInstanceDefs[i].CEntityDef.rotation;
					glm::mat4 matrix = glm::translate(glm::mat4(1.0f), map->CMloInstanceDefs[i].CEntityDef.position + EntityDef.position) * glm::mat4_cast(glm::quat(-map->CMloInstanceDefs[i].CEntityDef.rotation.w, -map->CMloInstanceDefs[i].CEntityDef.rotation.x, -map->CMloInstanceDefs[i].CEntityDef.rotation.y, -map->CMloInstanceDefs[i].CEntityDef.rotation.z)) * glm::scale(glm::mat4(1.0f), glm::vec3(EntityDef.scaleXY, EntityDef.scaleXY, EntityDef.scaleZ));

					LoadYDR(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].CEntityDef.position + EntityDef.position, 1.0f, matrix);
					//LoadYDD(camera, map->CMloInstanceDefs[i].CEntityDef.archetypeName, 0, it->second._BaseArchetypeDef.bsCentre + map->CEntityDefs[i].position, it->second._BaseArchetypeDef.bsRadius * std::max(map->CEntityDefs[i].scaleXY, map->CEntityDefs[i].scaleZ), it->second._BaseArchetypeDef.drawableDictionary, map->ModelMatrices[i]);
					//LoadYFT(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].CEntityDef.position + EntityDef.position, 100.0f, matrix);
				}
			}
		}
	}*/
	/*if (map->CCarGens.size() > 0) {
		for (auto& carGen : map->CCarGens)
		{
			int MaximumAvailableVehicles = 20 - vehicles.size(); //HARDCODED
			if (camera->Position.z < 100.0f) {
				for (int i = 0; i < MaximumAvailableVehicles; i++) {
					float xRandom = RandomFloat(carGen.position.x - carGen.perpendicularLength, carGen.position.x + carGen.perpendicularLength);
					float yRandom = RandomFloat(carGen.position.y - carGen.perpendicularLength, carGen.position.y + carGen.perpendicularLength);
					//if (!camera->intersects(glm::vec3(xRandom, yRandom, carGen.position.z), 1.0f)) {
						createVehicle(glm::vec3(xRandom, yRandom, carGen.position.z));
					//}
				}
			}
		}
	}*/

}

YmapLoader* GameWorld::GetYmap(uint32_t hash)
{
	auto it = ymapLoader.find(hash);
	if (it != ymapLoader.end()) {
		it->second->time = SDL_GetTicks();
		return it->second;
	}
	else {
		ymapLoader[hash] = ymapPool.Load();
		ymapLoader[hash]->time = SDL_GetTicks();
		GetResourceManager()->AddToWaitingList(new Resource(ymap, hash));
		return ymapLoader[hash];
	}
}

bool GameWorld::LoadYTYP(uint32_t hash)
{
	auto it = data.YtypEntries.find(hash);
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

YtdLoader* GameWorld::LoadYTD(uint32_t hash)
{
	auto it = ytdLoader.find(hash);
	if (it != ytdLoader.end())
	{
		it->second->time = SDL_GetTicks();
		return it->second;
	}
	else {
		ytdLoader[hash] = new YtdLoader();
		ytdLoader[hash]->time = SDL_GetTicks();
		GetResourceManager()->AddToWaitingList(new Resource(ytd, hash));

		return ytdLoader[hash];
	}
}

YdrLoader * GameWorld::GetYdr(uint32_t hash, uint32_t TextureDictionaryHash)
{
	auto iter = ydrLoader.find(hash);
	if (iter != ydrLoader.end())
	{
		if (iter->second->externalYtd)
			iter->second->externalYtd->time = SDL_GetTicks();
		iter->second->time = SDL_GetTicks();
		return iter->second;
	}
	else {
		ydrLoader[hash] = new YdrLoader();
		ydrLoader[hash]->time = SDL_GetTicks();
		GetResourceManager()->AddToWaitingList(new Resource(ydr, hash, TextureDictionaryHash));
		LoadYTD(TextureDictionaryHash);

		return ydrLoader[hash];
	}
}

YddLoader * GameWorld::GetYdd(uint32_t hash, uint32_t TextureDictionaryHash)
{
	auto iter = yddLoader.find(hash);
	if (iter != yddLoader.end())
	{
		if (iter->second->externalYtd)
			iter->second->externalYtd->time = SDL_GetTicks();
		iter->second->time = SDL_GetTicks();
		return iter->second;
	}
	else {
		yddLoader[hash] = new YddLoader();
		yddLoader[hash]->time = SDL_GetTicks();
		GetResourceManager()->AddToWaitingList(new Resource(ydd, hash, TextureDictionaryHash));
		LoadYTD(TextureDictionaryHash);

		return yddLoader[hash];
	}
}

YftLoader * GameWorld::GetYft(uint32_t hash, uint32_t TextureDictionaryHash)
{
	auto iter = yftLoader.find(hash);
	if (iter != yftLoader.end())
	{
		//if (iter->second->YdrFile->externalYtd)
			//iter->second->YdrFile->externalYtd->time = SDL_GetTicks();
		iter->second->time = SDL_GetTicks();
		return iter->second;
	}
	else {
		yftLoader[hash] = new YftLoader();
		yftLoader[hash]->time = SDL_GetTicks();
		GetResourceManager()->AddToWaitingList(new Resource(yft, hash, TextureDictionaryHash));
		LoadYTD(TextureDictionaryHash);

		return yftLoader[hash];
	}
}

void GameWorld::LoadYBN(uint32_t hash)
{
	std::unordered_map<uint32_t, YbnLoader*>::iterator iter = ybnLoader.find(hash);
	if (iter != ybnLoader.end())
	{
		iter->second->time = SDL_GetTicks();
		return;
	}
	else {
		ybnLoader[hash] = new YbnLoader();
		ybnLoader[hash]->time = SDL_GetTicks();
		GetResourceManager()->AddToWaitingList(new Resource(ybn, hash));
	}
}

void GameWorld::GetVisibleYmaps(Camera* camera)
{
	SpaceGridCell& cell = spaceGrid.GetCell(camera->Position);

	//DetectInWater(Position);

	for (auto& BoundsItem : cell.BoundsStoreItems)
	{
		LoadYBN(cacheFile.AllBoundsStoreItems[BoundsItem].Name);
	}

	for (auto& mapNode : cell.MapNodes)
	{
		LoadYmap(cacheFile.AllMapNodes[mapNode].Name, camera);
	}

	/*for (auto& Proxy : cell.CInteriorProxies)
	{
		LoadYmap(cacheFile.AllCInteriorProxies[Proxy].Parent, camera);
	}*/

	//printf("FREE VAO %zd\n",MeshManager::VAOs.size());
	//printf("FREE VBO %zd\n",MeshManager::VBOs.size());
	//printf("FREE Textures %zd\n", TextureManager::TexturesID.size());


	/*glm::i32vec2 test = nodeGrid.GetCellPos(camera->Position);

	if (nodeGrid.cells[test.x * 32 + test.y]->ynd) {

		for (auto& node : nodeGrid.cells[test.x * 32 + test.y]->ynd->nodes)
		{
			//printf("FOUND %f %f %f\n", node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f);
			UpdateTraffic(camera, glm::vec3(node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f));

		}
	}*/

	std::sort(renderList.begin(), renderList.end(), [&camera](RenderInstruction& a, RenderInstruction& b) { //FRONT_TO_BACK
		glm::vec3 lhsPosition = glm::vec3(a.modelMatrix[3]);
		glm::vec3 rhsPosition = glm::vec3(b.modelMatrix[3]);

		return glm::distance(lhsPosition, camera->Position) < glm::distance(rhsPosition, camera->Position);
	});


	//LoadYBN(Proxy->Name);
	//LoadYmap(Proxy->Parent, Position);

	//printf("CULLED :%d\n", culled);
	//culled = 0;

	uint32_t curTime = SDL_GetTicks();

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
			ymapPool.Remove(it->second);
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

	LoadQueuedResources();
}

void GameWorld::LoadQueuedResources()
{
	resources_lock.lock();
	for (auto it = resources.begin(); it != resources.end();)
	{
		memstream stream((*it)->Buffer.data(), (*it)->Buffer.size());

		switch ((*it)->type)
		{
			case ymap:
			{
				auto iter = ymapLoader.find((*it)->Hash);
				if (iter != ymapLoader.end())
				{
					iter->second->Init(stream);
					iter->second->time = SDL_GetTicks();
				}
				else {
					printf("");
				}
				break;
			}
			case ydr:
			{
				auto iter = ydrLoader.find((*it)->Hash);
				if (iter != ydrLoader.end())
				{
					YtdLoader* ytd = LoadYTD((*it)->TextureDictionaryHash);
					iter->second->Init(stream, (*it)->SystemSize, dynamicsWorld);
					iter->second->time = SDL_GetTicks();
					if (ytd) {
						iter->second->externalYtd = ytd;
						iter->second->externalYtd->time = SDL_GetTicks();
					}
				}
				else {
					printf("");
				}
				break;
			}
			case ydd:
			{
				auto iter = yddLoader.find((*it)->Hash);
				if (iter != yddLoader.end())
				{
					YtdLoader * ytd = LoadYTD((*it)->TextureDictionaryHash);
					iter->second->Init(stream, (*it)->SystemSize, dynamicsWorld);
					iter->second->time = SDL_GetTicks();
					if (ytd) {
						iter->second->externalYtd = ytd;
						iter->second->externalYtd->time = SDL_GetTicks();
					}
				}
				else {
					printf("");
				}
				break;
			}
			case yft:
			{
				auto iter = yftLoader.find((*it)->Hash);
				if (iter != yftLoader.end())
				{
					YtdLoader* ytd = LoadYTD((*it)->TextureDictionaryHash);
					iter->second->Init(stream, (*it)->SystemSize, false, dynamicsWorld);
					iter->second->time = SDL_GetTicks();
					if (ytd) {
						iter->second->YdrFile->externalYtd = ytd;
						iter->second->YdrFile->externalYtd->time = SDL_GetTicks();
					}
				}
				else {
					printf("");
				}
				break;
			}
			case ytd:
			{
				auto iter = ytdLoader.find((*it)->Hash);
				if (iter != ytdLoader.end())
				{
					iter->second->Init(stream, 0);
					iter->second->time = SDL_GetTicks();
				}
				else {
					printf("");
				}
				break;
			}
			case ybn:
			{
				auto iter = ybnLoader.find((*it)->Hash);
				if (iter != ybnLoader.end())
				{
					iter->second->Init(dynamicsWorld, stream);
					iter->second->time = SDL_GetTicks();
					break;
				}
				else {
					printf("");
				}
			}
		}

		delete *it;
		it = resources.erase(it);
	}
	resources_lock.unlock();
}

void GameWorld::createPedestrian()
{
	//Player *newPlayer = new Player(glm::vec3(0, 20, 0), dynamicsWorld);
	//pedestrians.push_back(newPlayer);
}

void GameWorld::createVehicle(glm::vec3 position)
{
	int vehicleID = rand() % vehiclesPool.size();

	auto it = vehiclesPool.begin();
	std::advance(it, vehicleID);
	if (it->second.file == nullptr) {
		it->second.file = GetYft(it->first, 0);
	}
	else {
		it->second.file->time = SDL_GetTicks();
		Vehicle *newVehicle = new Vehicle(position, it->second.mass, it->second.file, dynamicsWorld);
		vehicles.emplace_back(newVehicle);
	}
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

void GameWorld::UpdateTraffic(Camera* camera, glm::vec3 pos)
{
	float radiusTraffic = 20.0f;
	//PEDESTRIANS
	
	if (pedPool.firstAvailable_ == nullptr) {
		for (auto& ped : pedPool.peds)
		{
			if (glm::distance(camera->Position, glm::vec3(ped.getPhysCharacter()->getWorldTransform().getOrigin().getX(), ped.getPhysCharacter()->getWorldTransform().getOrigin().getY(), ped.getPhysCharacter()->getWorldTransform().getOrigin().getZ())) >= 100.0f) {
				pedPool.Remove(&ped);
			}
		}
	}

	while (pedPool.firstAvailable_ != nullptr)
	{
		float xRandom = RandomFloat(camera->Position.x - radiusTraffic, camera->Position.x + radiusTraffic);
		float yRandom = RandomFloat(camera->Position.y - radiusTraffic, camera->Position.y + radiusTraffic);
		//if (!camera->intersects(glm::vec3(xRandom, yRandom, camera->Position.z + 3.0f), 1.0f)) {
			pedPool.Add(glm::vec3(xRandom, yRandom, camera->Position.z + 3.0f), playerYDD, dynamicsWorld);
		//}
	}
	//CARS
	for (int i = 0; i < vehicles.size(); i++) {
		glm::vec3 vehiclePosition(vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getX(), vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getY(), vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getZ());
		if (glm::distance(camera->Position, vehiclePosition) >= 100.0f) {
			dynamicsWorld->removeVehicle((vehicles[i]->m_vehicle));
			dynamicsWorld->removeRigidBody((vehicles[i]->m_carChassis));
			delete vehicles[i];
			vehicles.erase(vehicles.begin() + i);
		}
	}
	uint32_t MaximumAvailableVehicles = 10 - vehicles.size(); //HARDCODED
	if (camera->Position.z < 100.0f) {
		for (uint32_t i = 0; i < MaximumAvailableVehicles; i++) {
			float xRandom = RandomFloat(pos.x - radiusTraffic, pos.x + radiusTraffic);
			float yRandom = RandomFloat(pos.y - radiusTraffic, pos.y + radiusTraffic);
			if (!camera->intersects(glm::vec3(xRandom, yRandom, pos.z), 1.0f)) {
				//createVehicle(glm::vec3(xRandom, yRandom, pos.z));
			}
		}
	}
}

Vehicle* GameWorld::FindNearestVehicle()
{
	float d = 15.0f;

	Vehicle* nearestVehicle = nullptr;

	for (auto& vehicle : vehicles)
	{
		glm::vec3 PlayerPosition(player[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getX(), player[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getY(), player[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getZ());
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
	glm::vec3 HitPos = CameraPosition + lookDirection;
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
			//player->getPhysCharacter()->applyImpulse(btVector3(0.0f, 0.0f, -20.0f));
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



void GameWorld::update(float delta_time, Camera* camera)
{
	Update();
	//UpdateTraffic(camera, camera->Position);
	dynamicsWorld->stepSimulation(delta_time);
	if (EnableStreaming) {
		renderList.clear();
		GetVisibleYmaps(camera);
	}
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
	printf("PRESSED");
	for (auto it = ybnLoader.begin(); it != ybnLoader.end();)
	{
		delete it->second;
		it = ybnLoader.erase(it);

	}

	for (auto it = ymapLoader.begin(); it != ymapLoader.end();)
	{
		delete it->second;
		it = ymapLoader.erase(it);
	}

	for (auto it = ydrLoader.begin(); it != ydrLoader.end();)
	{

		delete it->second;
		it = ydrLoader.erase(it);

	}

	for (auto it = yddLoader.begin(); it != yddLoader.end();)
	{

		delete it->second;
		it = yddLoader.erase(it);

	}

	for (auto it = yftLoader.begin(); it != yftLoader.end();)
	{

		delete it->second;
		it = yftLoader.erase(it);

	}

	for (auto it = ytdLoader.begin(); it != ytdLoader.end();)
	{

		delete it->second;
		it = ytdLoader.erase(it);

	}
}
