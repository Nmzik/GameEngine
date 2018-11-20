#include "GameWorld.h"

#include "ResourceManager.h"
#include "PhysicsDebugDrawer.h"
#include "Water.h"
#include "Camera.h"
#include "YtdLoader.h"
#include "YddLoader.h"
#include "YftLoader.h"
#include "YmapLoader.h"
#include "YtypLoader.h"
#include "YndLoader.h"
#include "YbnLoader.h"
#include "YnvLoader.h"
#include "Object.h"
#include "CPed.h"
#include "CVehicle.h"

btDiscreteDynamicsWorld* GameWorld::dynamicsWorld = nullptr;

GameWorld::GameWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, -9.8f));
	//UPDATE STATIC OBJECTS MANUALLY
	//dynamicsWorld->setForceUpdateAllAabbs(false);

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(&debug);
	//////////////////////////////////////////////////////////////////////////////

	_ResourceManager = std::make_unique<ResourceManager>(this);

	{
		std::vector<YdrLoader*> ydrs;
		ydrs.reserve(1500);

		uint32_t valuetest = 0;

		/*for (auto it = data.YdrEntries.begin(); valuetest < 1500; it++)
		{
			valuetest++;
			std::vector<uint8_t> Buffer(it->second->SystemSize + it->second->GraphicsSize);
			data.ExtractFileResource(*(it->second), Buffer);

			memstream stream(Buffer.data(), Buffer.size());
			YdrLoader* loader = new YdrLoader();
			loader->Init(stream, it->second->SystemSize);
			ydrs.push_back(loader);
		}

		for (int i = 0; i < 1500; i++)
		{
			ydrs[i]->Remove();
			delete ydrs[i];
			glFlush();
		}
		glFinish();*/
	}
	
		auto it = data.YdrEntries.find(3186271474);
		if (it != data.YdrEntries.end())
		{
			std::vector<uint8_t> Buffer(it->second->SystemSize + it->second->GraphicsSize);
			data.ExtractFileResource(*(it->second), Buffer);

			memstream stream(Buffer.data(), Buffer.size());
			YdrLoader* loader = new YdrLoader();
			loader->Init(stream, it->second->SystemSize);
			
			loader->Remove();
			
			delete loader;
			glFinish();
		}
	/*auto YnvIt = data.YnvEntries.find(1471038032);
	if (YnvIt != data.YnvEntries.end()) {
		std::vector<uint8_t> Buffer(YnvIt->second->SystemSize + YnvIt->second->GraphicsSize);
		data.ExtractFileResource(*(YnvIt->second), Buffer);
		//YnvLoader ynv(stream);
	}

	//Nodes
	/*for (int x = 0; x < nodeGrid.CellCountX; x++)
	{
		for (int y = 0; y < nodeGrid.CellCountY; y++)
		{
			std::string filename = "nodes" + std::to_string(nodeGrid.cells[x * nodeGrid.CellCountX + y]->ID);
			uint32_t fnhash = GenHash(filename);

			auto it = data.YndEntries.find(fnhash);
			if (it != data.YndEntries.end()) {

				auto& element = *(it->second);
				std::vector<uint8_t> outputBuffer;
				outputBuffer.resize(it->second->SystemSize + it->second->GraphicsSize);
				data.ExtractFileResource(element, outputBuffer);

				memstream stream(outputBuffer.data(), outputBuffer.size());

				YndLoader* ynd = myNew YndLoader(stream);
				nodeGrid.cells[x * nodeGrid.CellCountX + y]->ynd = ynd;
			}
		}
	}*/

	for (int i = 0; i < data.cacheFile->AllMapNodes.size(); i++)
	{
		if (data.cacheFile->AllMapNodes[i].Unk1 == 1) //NOT SURE
			spaceGrid.AddMapNode(&data.cacheFile->AllMapNodes[i], i);
	}

	for (int i = 0; i < data.cacheFile->AllBoundsStoreItems.size(); i++)
	{
		spaceGrid.AddBoundsItem(&data.cacheFile->AllBoundsStoreItems[i], i);
	}

	for (int i = 0; i < data.cacheFile->AllCInteriorProxies.size(); i++)
	{
		spaceGrid.AddCInteriorProxy(&data.cacheFile->AllCInteriorProxies[i], i);
	}

	for (auto& vehicle : data.Vehicles)
	{
		vehiclesPool[vehicle.Hash] = vehicle;
	}

	peds.reserve(20);

	//RenderList
	renderList.reserve(2000);
	//
    while (!_ResourceManager->GetYtd(3403519606)->Loaded)
	{
		LoadQueuedResources();
	}

	for (auto& WaterQuad : data.WaterQuads)
	{
		Water water(WaterQuad);
		WaterMeshes.push_back(water);
	}

	gameHour = 10;
	gameMinute = 0;

	/*auto it = getGameData()->YmapEntries.find(3606201153);
	if (it != getGameData()->YmapEntries.end())
	{
		YmapLoader* loader = YmapPool::getPool().Load();
		
		std::vector<uint8_t> Buffer(it->second->SystemSize + it->second->GraphicsSize);
		getGameData()->ExtractFileResource(*(it->second), Buffer);

		memstream stream(Buffer.data(), Buffer.size());
		loader->Init(stream);
	}*/

	while (!_ResourceManager->GetYtd(4096714883)->Loaded)
	{
		LoadQueuedResources();
	}

	YddLoader* playerYDD = _ResourceManager->GetYdd(4096714883, 4096714883);

	skydome = _ResourceManager->GetYdd(2640562617, 2640562617);

	_ResourceManager->GetYtd(GenHash("mapdetail"));
    _ResourceManager->GetYtd(GenHash("vehshare"));
    _ResourceManager->GetYtd(GenHash("vehshare_worn"));
    _ResourceManager->GetYtd(GenHash("vehshare_army"));
    _ResourceManager->GetYtd(GenHash("vehshare_truck"));

	/*for (auto& ytd : data.GtxdEntries)
	{
		while (!GetYtd(ytd.second)->Loaded)
		{
			LoadQueuedResources();
		}
	}*/

	for (auto& vehicle : vehiclesPool)
	{
        _ResourceManager->GetYtd(vehicle.first);
	}

	while (!skydome->Loaded || !playerYDD->Loaded) {
		LoadQueuedResources();
	}

	peds.emplace_back(glm::vec3(-2020.47, -449.58, 19.71), playerYDD, dynamicsWorld);
	peds.emplace_back(glm::vec3(9.66, -1184.98, 75.74), playerYDD, dynamicsWorld);
	peds.emplace_back(glm::vec3(2250.18f, 3471.40f, 56.50f), playerYDD, dynamicsWorld);

	//YdrLoader* test = GetFile<YdrLoader, YdrPool, Type::ydr>(4234, 42423);
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
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	//delete dynamicsWorld;
}

float RandomFloat(float min, float max) {
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

void GameWorld::LoadYmap(YmapLoader* map, Camera* camera, glm::vec3& position)
{
	if (map->Loaded) {
		for (auto& object : *map->Objects)
		{
			float Dist = glm::length2(position - object.position);
			bool IsVisible = Dist <= object.CEntity.lodDist * LODMultiplier;
			bool childrenVisible = (Dist <= object.CEntity.childLodDist * LODMultiplier) && (object.CEntity.numChildren > 0);
			if (IsVisible && !childrenVisible) {
				if (!object.Loaded) {
					switch (object.Archetype->BaseArchetypeDef.assetType)
					{
						case ASSET_TYPE_DRAWABLE: {
							if (!object.FoundModel) {
								object.ydr = _ResourceManager->GetYdr(object.CEntity.archetypeName, object.Archetype->BaseArchetypeDef.textureDictionary);
								object.ytd = _ResourceManager->GetYtd(object.Archetype->BaseArchetypeDef.textureDictionary);
								object.FoundModel = true;
							}
							if (object.ydr->Loaded) {
								//NOTE:
								//
								//SPAWN OBJECTS STATICALLY (IN SLEEP STATE)
								//
								//SUPER DIRTY NEED FIX URGENT! UGLY FIX!!!
								/*if (object.ydr->ybnfile) {

									if (object.ydr->ybnfile->compound->getNumChildShapes() != 0) {

										//SET POSITION OF COLLISION TO OBJECT PLACE
										//NOTE: SPAWN AT EXACT SAME PLACE!!! "NO +1.0f"
										btVector3 localInertia(0, 0, 0);
										float mass = 0.0f;
										
										btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(-object.rotation.x, object.rotation.y, object.rotation.z, object.rotation.w), btVector3(object.position.x, object.position.y, object.position.z)));
										btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, MotionState, object.ydr->ybnfile->compound, localInertia);
										object.rigidBody = new btRigidBody(groundRigidBodyCI);
										dynamicsWorld->addRigidBody(object.rigidBody);
									}//can be an error here
								}*/

								object.Loaded = true;
							}
							break;
						}
						case ASSET_TYPE_DRAWABLEDICTIONARY: {
							if (!object.FoundModel) {
								object.ydd = _ResourceManager->GetYdd(object.Archetype->BaseArchetypeDef.drawableDictionary, object.Archetype->BaseArchetypeDef.textureDictionary);
								object.ytd = _ResourceManager->GetYtd(object.Archetype->BaseArchetypeDef.textureDictionary);
								object.FoundModel = true;
							}
							if (object.ydd->Loaded) {
								auto iter2 = object.ydd->YdrFiles->find(object.CEntity.archetypeName);
								if (iter2 != object.ydd->YdrFiles->end())
								{
									object.ydr = iter2->second;
									object.Loaded = true;
								}
							}
							break;
						}
						case ASSET_TYPE_FRAGMENT: {
							if (!object.FoundModel) {
								object.yft = _ResourceManager->GetYft(object.CEntity.archetypeName, object.Archetype->BaseArchetypeDef.textureDictionary);
								object.ytd = _ResourceManager->GetYtd(object.Archetype->BaseArchetypeDef.textureDictionary);
								object.FoundModel = true;
							}
							if (object.yft->Loaded) {
								object.ydr = object.yft->YdrFile;

								/*if (object.yft->ybnFile) {
									if (object.yft->ybnFile->compound->getNumChildShapes() != 0) {

										//SET POSITION OF COLLISION TO OBJECT PLACE
										btVector3 localInertia(0, 0, 0);
										float mass = 0.0f;
										btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(-object.rotation.x, object.rotation.y, object.rotation.z, object.rotation.w), btVector3(object.position.x, object.position.y, object.position.z)));
										btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, MotionState, object.yft->ybnFile->compound, localInertia);
										object.rigidBody = new btRigidBody(groundRigidBodyCI);
										dynamicsWorld->addRigidBody(object.rigidBody);
									}//can be an error here
								}*/

								object.Loaded = true;
							}
							break;
						}
					}
				}
				else {

					if ((object.Archetype->BaseArchetypeDef.flags & 2048) > 0)
					{
						//if (!renderProxies) continue;
						continue;
					}

					if (object.Archetype->GetType() == 1) { //TIME ARCHETYPE
						//if ((object.Archetype._TimeArchetypeDef.timeFlags >> gameHour) & 1)
						//{
						continue;
						//}
					}

					if (camera->intersects(object.BoundPos, object.BoundRadius)) {
						renderList.emplace_back(object.ydr, object.getMatrix());
					}
				}
			}
		}
	}
	/*if (map->CMloInstanceDefs.size() > 0) {
		for (int i = 0; i < map->CMloInstanceDefs.size(); i++)
		{
			auto it = data.MloDictionary.find(map->CMloInstanceDefs[i].fwEntityDef.archetypeName);
			if (it != data.MloDictionary.end())
			{
				for (auto& EntityDef : it->second)
				{
					glm::vec4 rotmultiply = EntityDef.rotation * map->CMloInstanceDefs[i].fwEntityDef.rotation;
					glm::mat4 matrix = glm::translate(glm::mat4(1.0f), map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position) * glm::mat4_cast(glm::quat(-map->CMloInstanceDefs[i].fwEntityDef.rotation.w, -map->CMloInstanceDefs[i].fwEntityDef.rotation.x, -map->CMloInstanceDefs[i].fwEntityDef.rotation.y, -map->CMloInstanceDefs[i].fwEntityDef.rotation.z)) * glm::scale(glm::mat4(1.0f), glm::vec3(EntityDef.scaleXY, EntityDef.scaleXY, EntityDef.scaleZ));

					LoadYDR(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position, 1.0f, matrix);
					//LoadYDD(camera, map->CMloInstanceDefs[i].fwEntityDef.archetypeName, 0, it->second._BaseArchetypeDef.bsCentre + map->fwEntityDefs[i].position, it->second._BaseArchetypeDef.bsRadius * std::max(map->fwEntityDefs[i].scaleXY, map->fwEntityDefs[i].scaleZ), it->second._BaseArchetypeDef.drawableDictionary, map->ModelMatrices[i]);
					//LoadYFT(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position, 100.0f, matrix);
				}
			}
		}
	}*/

	//NOTE::
	//
	//RANDOM VEHICLE REALLY SPAWNS in RANDOM PLACE on the same CCarGens IN real game (spawns/despawns based on distance to player, checked on the same place)
	//
	//
	//
	/*if (map->CCarGens.size() > 0) {
		for (auto& carGen : map->CCarGens)
		{
			int MaximumAvailableVehicles = 20 - vehicles.size(); //HARDCODED
			if (position.z < 100.0f) {
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

/*bool GameWorld::LoadYTYP(uint32_t hash)
{
	auto it = data.YtypEntries.find(hash);
	if (it != data.YtypEntries.end())
	{
		std::cout << "YTYP Found " << it->second->Name << std::endl;
		auto& element = *(it->second);
		std::vector<uint8_t> outputBuffer;
		data.ExtractFileResource(element, outputBuffer);

		memstream stream(outputBuffer.data(), outputBuffer.size());

		YtypLoader* file = myNew YtypLoader(stream);
		ytypLoader.push_back(file);

		return true;
	}
}*/

void GameWorld::GetVisibleYmaps(Camera* camera)
{
	glm::vec3 PlayerPos = peds[currentPlayerID].getPos();
	//glm::vec3 PlayerPos = camera->position;

	auto cellID = spaceGrid.GetCellPos(PlayerPos);
	auto NodeCell = nodeGrid.GetCellPos(PlayerPos);

	if (CurNodeCell != NodeCell) {
		CurNodeCell = NodeCell;
	}

	if (CurCell != cellID)
	{
		for (auto& ybn : CurYbns)
		{
			ybn->RefCount--;
		}

		for (auto& map : CurYmaps)
		{
			map->RefCount--;
		}

		CurYbns.clear();
		CurYmaps.clear();
		//Clear previous Ybns

		//printf("NEW CELL\n");
		CurCell = cellID;

		SpaceGridCell& cell = spaceGrid.GetCell(cellID);

		for (auto& BoundsItem : cell.BoundsStoreItems)
		{
			CurYbns.emplace_back(_ResourceManager->GetYbn(data.cacheFile->AllBoundsStoreItems[BoundsItem].Name));
		}

		for (auto& mapNode : cell.MapNodes)
		{
            CurYmaps.emplace_back(_ResourceManager->GetYmap(data.cacheFile->AllMapNodes[mapNode].Name));
		}

		_ResourceManager->UpdateResourceCache();
	}

	for (auto& mapNode : CurYmaps)
	{
		LoadYmap(mapNode, camera, PlayerPos);
	}

	/*for (auto& Proxy : cell.CInteriorProxies)
	{
		LoadYmap(cacheFile.AllCInteriorProxies[Proxy].Parent, camera);
	}*/

	//printf("FREE VAO %zd\n",MeshManager::VAOs.size());
	//printf("FREE VBO %zd\n",MeshManager::VBOs.size());
	//printf("FREE Textures %zd\n", TextureManager::TexturesID.size());
	//printf("SIZE OBJECTS %zd\n",renderList.size());

	/*glm::i32vec2 test = nodeGrid.GetCellPos(camera->position);

	if (nodeGrid.cells[test.x * 32 + test.y]->ynd) {

		for (auto& node : nodeGrid.cells[test.x * 32 + test.y]->ynd->nodes)
		{
			//printf("FOUND %f %f %f\n", node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f);
			UpdateTraffic(camera, glm::vec3(node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f));

		}
	}*/

	for (auto& ped : peds)
	{
		//if ped loaded
		auto& model = ped.getPosition();
		if (camera->intersects(ped.position, 1.0f)) {

			for (auto& ydr : ped.playerModel)
			{
				renderList.emplace_back(ydr, model);
			}
		}
	}

	std::sort(renderList.begin(), renderList.end(), [&camera](RenderInstruction& a, RenderInstruction& b) { //FRONT_TO_BACK
		glm::vec3 lhsPosition = glm::vec3(a.modelMatrix[3]);
		glm::vec3 rhsPosition = glm::vec3(b.modelMatrix[3]);

		return glm::distance2(lhsPosition, camera->position) < glm::distance2(rhsPosition, camera->position);
	});

	//printf("YDRS %d\n", YdrPool::getPool().num);
	//printf("SIZE YMAP %zd\n", ymapLoader.size());
	//printf("SIZE YDR %zd\n", ydrLoader.size());
	//printf("SIZE YDD %zd\n", yddLoader.size());
	//printf("SIZE YFT %zd\n", yftLoader.size());
	//printf("SIZE YTD %zd\n", ytdLoader.size());
	//printf("TexturesMapsSize %zd\n", TextureManager::TexturesMap.size());
	//LoadYBN(Proxy->Name);
	//LoadYmap(Proxy->Parent, Position);

	//printf("CULLED :%d\n", ydrLoader.size());
	//culled = 0;

	/*auto& TexManager = TextureManager::GetTextureManager();

	for (auto it = TexManager.TexturesMap.begin(); it != TexManager.TexturesMap.begin();)
	{
		if (it->second.referenceCount <= 0)
		{
			printf("");
		}
		/*else
		{
			++it;
		}
	}*/

	LoadQueuedResources();
}

void GameWorld::LoadQueuedResources()
{
	resources_lock.lock();
	while (resources.size() > 0)
	{
		Resource res = std::move(resources.back());
		resources.pop_back();

		/*if (res.Hash == 3486509883) {
		printf("");
		}*/


		//Object hash equal to texture hash what should we do? there are +hi textures with the same name 

		/*if (res.type == ydr || res.type == ydd || res.type == yft) {
			if (!res.TextureDictionaryHash == 0) {
				auto iter = ytdLoader.find(res.TextureDictionaryHash);
				if (iter != ytdLoader.end())
				{
					if (!iter->second->Loaded) {
						++it;
						continue;
					}
				}
			}
		}*/

		if (res.Buffer.size() == 0) {
			res.file->Loaded = true;
		}
		else {
			memstream stream(res.Buffer.data(), res.Buffer.size());
			switch (res.type)
			{
				case ymap:
				{
					res.file->Finalize();
					break;
				}
				case ydr:
				case ydd:
				case yft:
				{
					res.file->Init(stream, res.SystemSize);
					_ResourceManager->GlobalGpuMemory += res.file->gpuMemory;
					break;
				}
				case ytd:
				{
					res.file->Init(stream, res.SystemSize);
                    _ResourceManager->TextureMemory += res.file->gpuMemory;
					break;
				}
				case ybn:
				{
					res.file->Finalize(dynamicsWorld);
					break;
				}
			}
		}
	}
	resources_lock.unlock();
}

void GameWorld::createPedestrian()
{
	//Player *newPlayer = myNew Player(glm::vec3(0, 20, 0), dynamicsWorld);
	//peds.push_back(newPlayer);
}

void GameWorld::createVehicle(glm::vec3 position)
{
	int vehicleID = rand() % vehiclesPool.size();

	auto it = vehiclesPool.begin();
	std::advance(it, vehicleID);
	if (it->second.file == nullptr) {
		it->second.file = _ResourceManager->GetYft(it->first, it->first);
	}
	else {
		CVehicle veh(position, it->second.mass, it->second.file, dynamicsWorld);
		vehicles.push_back(veh);
	}
}

void GameWorld::UpdateDynamicObjects()
{
	/*for (auto & map : CurYmaps)
	{
		if (map->Loaded) {
			for (auto & object : map->Objects)
			{
				if (object.Loaded) {
					if (object.Archetype._BaseArchetypeDef.flags == 549584896 && object.rigidBody) {
						object.rigidBody->getWorldTransform().getOpenGLMatrix(&object.modelMatrix[0][0]);
					}
				}
			}
		}
	}*/
}

void GameWorld::Update()
{
	for (auto& pedestrian : peds)
	{
		pedestrian.PhysicsTick();
	}

	for (auto& vehicle : vehicles)
	{
		vehicle.PhysicsTick();
	}
}

void GameWorld::UpdateTraffic(Camera* camera, glm::vec3 pos)
{
	float radiusTraffic = 5.0f;
	//peds

	/*if (pedPool.firstAvailable_ == nullptr) {
		for (auto& ped : pedPool.peds)
		{
			if (glm::distance(camera->position, glm::vec3(ped.getPhysCharacter()->getWorldTransform().getOrigin().getX(), ped.getPhysCharacter()->getWorldTransform().getOrigin().getY(), ped.getPhysCharacter()->getWorldTransform().getOrigin().getZ())) >= 100.0f) {
				pedPool.Remove(&ped);
			}
		}
	}

	while (pedPool.firstAvailable_ != nullptr)
	{
		float xRandom = RandomFloat(camera->position.x - radiusTraffic, camera->position.x + radiusTraffic);
		float yRandom = RandomFloat(camera->position.y - radiusTraffic, camera->position.y + radiusTraffic);
		//if (!camera->intersects(glm::vec3(xRandom, yRandom, camera->position.z + 3.0f), 1.0f)) {
			//pedPool.Add(glm::vec3(xRandom, yRandom, camera->position.z + 3.0f), playerYDD, dynamicsWorld);
		//}
	}*/
	//CARS
	if (nodeGrid.cells[CurNodeCell.x * 32 + CurNodeCell.y]->ynd) {

		for (auto& node : nodeGrid.cells[CurNodeCell.x * 32 + CurNodeCell.y]->ynd->nodes)
		{
			pos = glm::vec3(node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f);

			for (int i = 0; i < vehicles.size(); i++) {
				glm::vec3 vehiclePosition(vehicles[i].m_carChassis->getWorldTransform().getOrigin().getX(), vehicles[i].m_carChassis->getWorldTransform().getOrigin().getY(), vehicles[i].m_carChassis->getWorldTransform().getOrigin().getZ());
				if (glm::distance(camera->position, vehiclePosition) >= 100.0f) {
					dynamicsWorld->removeVehicle((vehicles[i].m_vehicle));
					dynamicsWorld->removeRigidBody((vehicles[i].m_carChassis));
					vehicles.erase(vehicles.begin() + i);
				}
			}
			uint32_t MaximumAvailableVehicles = 30 - vehicles.size(); //HARDCODED
			if (camera->position.z < 100.0f) {
				for (uint32_t i = 0; i < MaximumAvailableVehicles; i++) {
					float xRandom = RandomFloat(pos.x - radiusTraffic, pos.x + radiusTraffic);
					float yRandom = RandomFloat(pos.y - radiusTraffic, pos.y + radiusTraffic);
					if (!camera->intersects(glm::vec3(xRandom, yRandom, pos.z), 1.0f)) {
						createVehicle(glm::vec3(xRandom, yRandom, pos.z));
					}
				}
			}

		}
	}
}

CVehicle* GameWorld::FindNearestVehicle()
{
	float d = 15.0f;

	CVehicle* nearestVehicle = nullptr;

	for (auto& vehicle : vehicles)
	{
		glm::vec3 PlayerPosition(peds[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getX(), peds[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getY(), peds[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getZ());
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
	glm::vec3 HitPos = CameraPosition + lookDirection;
	btVector3 from(CameraPosition.x, CameraPosition.y, CameraPosition.z),
		to(HitPos.x, HitPos.y, HitPos.z);
	btCollisionWorld::ClosestRayResultCallback cb(from, to);
	cb.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	dynamicsWorld->rayTest(from, to, cb);

	if (cb.hasHit()) {

		CPed* player = static_cast<CPed*>(cb.m_collisionObject->getUserPointer());
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

constexpr float deltaTime = 1.f / 120.f;

void GameWorld::update(float delta_time, Camera* camera)
{
	dynamicsWorld->stepSimulation(delta_time, 10);

	/*if (delta_time > 0.25f) {
		delta_time = 0.25f;
	}

	accumulatedTime += delta_time;

	while (accumulatedTime >= deltaTime) {
		dynamicsWorld->stepSimulation(deltaTime, 2, deltaTime);

		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; ++i)
		{
			btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
			btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j < numContacts; j++)
			{

				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getAppliedImpulse() > 1) {

					Player* player1 = static_cast<Player*>(obA->getUserPointer());
					Player* player2 = static_cast<Player*>(obB->getUserPointer());

					if (player1 != nullptr) {
						player1->TakeDamage(pt.getAppliedImpulse());
					}
					else if (player2 != nullptr) {
						player2->TakeDamage(pt.getAppliedImpulse());
					}
				}
				/*if (pt.getDistance()<0.f)
				{
					if (pt.getAppliedImpulse() != 0) {
						printf("");
					}

				}
			}

		}

		accumulatedTime -= deltaTime;
	}*/

	UpdateDynamicObjects();
	//UpdateTraffic(camera, camera->position);
	Update();

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

}
