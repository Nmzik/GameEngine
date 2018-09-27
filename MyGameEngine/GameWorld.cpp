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

GameWorld::GameWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, -9.8f));
	//dynamicsWorld->setForceUpdateAllAabbs(false);

	debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(&debug);
	//////////////////////////////////////////////////////////////////////////////

	_ResourceManager = std::make_unique<ResourceManager>(this);

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

	ydrLoader.reserve(500);
	yddLoader.reserve(500);;
	yftLoader.reserve(500);
	ytdLoader.reserve(1500);
	//yndLoader.reserve(500);
	ybnLoader.reserve(50);
	//ytypLoader.reserve(500);
	ymapLoader.reserve(500);

	pedestrians.reserve(20);

	//RenderList
	renderList.reserve(2000);

	//TextureManager::Initialize();

	//Default texture
	while (!LoadYTD(3570112701)->Loaded)
	{
		LoadQueuedResources();
	}
	TextureManager::GetTextureManager().LoadTexture(1551155749, TextureManager::GetTextureManager().GetTexture(475118591));

	//
	while (!LoadYTD(3403519606)->Loaded)
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

	while (!LoadYTD(4096714883)->Loaded)
	{
		LoadQueuedResources();
	}

	YddLoader* playerYDD = GetYdd(4096714883, 4096714883);

	skydome = GetYdd(2640562617, 2640562617);

	LoadYTD(GenHash("mapdetail"));
	LoadYTD(GenHash("vehshare"));
	LoadYTD(GenHash("vehshare_worn"));
	LoadYTD(GenHash("vehshare_army"));
	LoadYTD(GenHash("vehshare_truck"));

	for (auto& ytd : data.GtxdEntries)
	{
		while (!LoadYTD(ytd.second)->Loaded)
		{
			LoadQueuedResources();
		}
	}

	for (auto& vehicle : vehiclesPool)
	{
		LoadYTD(vehicle.first);
	}

	while (!skydome->Loaded || !playerYDD->Loaded) {
		LoadQueuedResources();
	}


	pedestrians.emplace_back(glm::vec3(1982.886353, 3833.829102, 32.140667), playerYDD, dynamicsWorld);
	pedestrians.emplace_back(glm::vec3(9.66, -1184.98, 75.74), playerYDD, dynamicsWorld);
	pedestrians.emplace_back(glm::vec3(2250.18f, 3471.40f, 56.50f), playerYDD, dynamicsWorld);

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
	
}

float RandomFloat(float min, float max) {
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

void GameWorld::LoadYmap(YmapLoader* map, Camera* camera, glm::vec3& position)
{
	if (map->Loaded) {
		for (auto& object : map->Objects)
		{
			float Dist = glm::length2(position - object.position);
			bool IsVisible = Dist <= object.CEntity.lodDist * LODMultiplier;
			bool childrenVisible = (Dist <= object.CEntity.childLodDist * LODMultiplier) && (object.CEntity.numChildren > 0);
			if (IsVisible && !childrenVisible) {
				if (!object.Loaded) {
					switch (object.Archetype._BaseArchetypeDef.assetType)
					{
						case ASSET_TYPE_DRAWABLE: {
							if (!object.FoundModel) {
								object.ydr = GetYdr(object.CEntity.archetypeName, object.Archetype._BaseArchetypeDef.textureDictionary);
								object.ytd = LoadYTD(object.Archetype._BaseArchetypeDef.textureDictionary);
								object.FoundModel = true;
							}
							if (object.ydr->Loaded) {
								//SUPER DIRTY NEED FIX URGENT! UGLY FIX!!!
								if (object.ydr->ybnfile) {

									if (object.ydr->ybnfile->compound->getNumChildShapes() != 0) {

										//SET POSITION OF COLLISION TO OBJECT PLACE
										btVector3 localInertia(0, 0, 0);
										float mass = 0.0f;
										if (object.Archetype._BaseArchetypeDef.flags == 549584896) { //DYNAMIC???
											mass = 1.0f;
											object.ydr->ybnfile->compound->calculateLocalInertia(mass, localInertia);
										}
										btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(-object.rotation.x, object.rotation.y, object.rotation.z, object.rotation.w), btVector3(object.position.x, object.position.y, object.position.z + 1.0f)));
										btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, MotionState, object.ydr->ybnfile->compound, localInertia);
										object.rigidBody = new btRigidBody(groundRigidBodyCI);
										dynamicsWorld->addRigidBody(object.rigidBody);
									}//can be an error here
								}

								object.Loaded = true;
							}
							break;
						}
						case ASSET_TYPE_DRAWABLEDICTIONARY: {
							if (!object.FoundModel) {
								object.ydd = GetYdd(object.Archetype._BaseArchetypeDef.drawableDictionary, object.Archetype._BaseArchetypeDef.textureDictionary);
								object.ytd = LoadYTD(object.Archetype._BaseArchetypeDef.textureDictionary);
								object.FoundModel = true;
							}
							if (object.ydd->Loaded) {
								std::unordered_map<uint32_t, YdrLoader*>::iterator iter2 = object.ydd->YdrFiles.find(object.CEntity.archetypeName);
								if (iter2 != object.ydd->YdrFiles.end())
								{
									object.ydr = iter2->second;
									object.Loaded = true;
								}
							}
							break;
						}
						case ASSET_TYPE_FRAGMENT: {
							if (!object.FoundModel) {
								object.yft = GetYft(object.CEntity.archetypeName, object.Archetype._BaseArchetypeDef.textureDictionary);
								object.ytd = LoadYTD(object.Archetype._BaseArchetypeDef.textureDictionary);
								object.FoundModel = true;
							}
							if (object.yft->Loaded) {
								object.ydr = object.yft->YdrFile;
								object.Loaded = true;
							}
							break;
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

YmapLoader* GameWorld::GetYmap(uint32_t hash)
{
	auto it = ymapLoader.find(hash);
	if (it != ymapLoader.end()) {
		it->second->RefCount++;
		return it->second;
	}
	else {
		YmapLoader* loader = YmapPool::getPool().Load();
		GetResourceManager()->AddToWaitingList(myNew Resource(ymap, hash, 0, loader));
		loader->RefCount++;
		ymapLoader.insert({ hash, loader });

		return loader;
	}
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

		memstream2 stream(outputBuffer.data(), outputBuffer.size());

		YtypLoader* file = myNew YtypLoader(stream);
		ytypLoader.push_back(file);

		return true;
	}
}*/

void GameWorld::LoadGtxd(uint32_t hash)
{
	auto iter = data.GtxdEntries.find(hash);
	if (iter != data.GtxdEntries.end()) {
		auto it = ytdLoader.find(iter->second);
		if (it == ytdLoader.end()) {
			ytdLoader[iter->second] = myNew YtdLoader();
			GetResourceManager()->AddToWaitingList(myNew Resource(ytd, iter->second, 0, nullptr));
		}
	}
}

YtdLoader* GameWorld::LoadYTD(uint32_t hash)
{
	auto it = ytdLoader.find(hash);
	if (it != ytdLoader.end())
	{
		it->second->RefCount++;
		return it->second;
	}
	else {
		//LoadGtxd(hash);
		YtdLoader* loader = YtdPool::getPool().Load();
		GetResourceManager()->AddToWaitingList(myNew Resource(ytd, hash, 0, loader));
		loader->RefCount++;
		ytdLoader.insert({ hash, loader });

		return loader;
	}
}

YdrLoader * GameWorld::GetYdr(uint32_t hash, uint32_t TextureDictionaryHash)
{
	auto iter = ydrLoader.find(hash);
	if (iter != ydrLoader.end())
	{
		iter->second->RefCount++;
		return iter->second;
	}
	else {
		YdrLoader* loader = YdrPool::getPool().Load();
		GetResourceManager()->AddToWaitingList(myNew Resource(ydr, hash, TextureDictionaryHash, loader));
		loader->RefCount++;
		ydrLoader.insert({ hash, loader });

		return loader;
	}
}

YddLoader * GameWorld::GetYdd(uint32_t hash, uint32_t TextureDictionaryHash)
{
	auto iter = yddLoader.find(hash);
	if (iter != yddLoader.end())
	{
		iter->second->RefCount++;
		return iter->second;
	}
	else {
		YddLoader* loader = YddPool::getPool().Load();
		GetResourceManager()->AddToWaitingList(myNew Resource(ydd, hash, TextureDictionaryHash, loader));
		loader->RefCount++;
		yddLoader.insert({ hash, loader });

		return loader;
	}
}

YftLoader * GameWorld::GetYft(uint32_t hash, uint32_t TextureDictionaryHash)
{
	auto iter = yftLoader.find(hash);
	if (iter != yftLoader.end())
	{
		iter->second->RefCount++;
		return iter->second;
	}
	else {
		YftLoader* loader = YftPool::getPool().Load();
		GetResourceManager()->AddToWaitingList(myNew Resource(yft, hash, TextureDictionaryHash, loader));
		loader->RefCount++;
		yftLoader.insert({ hash, loader });

		return loader;
	}
}

YbnLoader* GameWorld::GetYBN(uint32_t hash)
{
	std::unordered_map<uint32_t, YbnLoader*>::iterator iter = ybnLoader.find(hash);
	if (iter != ybnLoader.end())
	{
		iter->second->RefCount++;
		return iter->second;
	}
	else {
		YbnLoader* loader = YbnPool::getPool().Load();
		GetResourceManager()->AddToWaitingList(myNew Resource(ybn, hash, 0, loader));
		loader->RefCount++;
		ybnLoader.insert({ hash, loader });
		return loader;
	}
}

void GameWorld::GetVisibleYmaps(Camera* camera)
{
	glm::vec3 PlayerPos = pedestrians[currentPlayerID].getPos();

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
			CurYbns.emplace_back(GetYBN(data.cacheFile->AllBoundsStoreItems[BoundsItem].Name));
		}

		for (auto& mapNode : cell.MapNodes)
		{
			CurYmaps.emplace_back(GetYmap(data.cacheFile->AllMapNodes[mapNode].Name));
		}
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

	for (auto& ped : pedestrians)
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

	for (auto it = ybnLoader.begin(); it != ybnLoader.end();)
	{
		if ((it->second)->RefCount == 0 && (it->second)->Loaded)
		{
			YbnLoader* ybn = (it->second);
			if (ybn->rigidBody) {
				delete ybn->rigidBody->getMotionState();

				dynamicsWorld->removeRigidBody(ybn->rigidBody);

				delete ybn->rigidBody;

				ybn->rigidBody = nullptr;
			}

			YbnPool::getPool().Remove(it->second);
			it = ybnLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = ymapLoader.begin(); it != ymapLoader.end();)
	{
		if ((it->second)->RefCount == 0 && (it->second)->Loaded)
		{
			YmapPool::getPool().Remove(it->second, dynamicsWorld);
			it = ymapLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = ydrLoader.begin(); it != ydrLoader.end();)
	{
		if ((it->second)->RefCount == 0 && (it->second)->Loaded)
		{
			GlobalGpuMemory -= it->second->gpuMemory;
			YdrPool::getPool().Remove(it->second);
			it = ydrLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = yddLoader.begin(); it != yddLoader.end();)
	{
		if ((it->second)->RefCount == 0 && (it->second)->Loaded)
		{
			GlobalGpuMemory -= it->second->gpuMemory;
			YddPool::getPool().Remove(it->second);
			it = yddLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = yftLoader.begin(); it != yftLoader.end();)
	{
		if ((it->second)->RefCount == 0 && (it->second)->Loaded)
		{
			GlobalGpuMemory -= it->second->gpuMemory;
			YftPool::getPool().Remove(it->second);
			it = yftLoader.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = ytdLoader.begin(); it != ytdLoader.end();)
	{
		if ((it->second)->RefCount == 0 && (it->second)->Loaded)
		{
			TextureMemory -= it->second->gpuMemory;
			YtdPool::getPool().Remove(it->second);
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
		memstream2 stream((*it)->Buffer.data(), (*it)->Buffer.size());

		/*if ((*it)->Hash == 3486509883) {
		printf("");
		}*/


		//Object hash equal to texture hash what should we do? there are +hi textures with the same name 

		if ((*it)->type == ydr || (*it)->type == ydd || (*it)->type == yft) {
			if (!(*it)->TextureDictionaryHash == 0) {
				auto iter = ytdLoader.find((*it)->TextureDictionaryHash);
				if (iter != ytdLoader.end())
				{
					if (!iter->second->Loaded) {
						++it;
						continue;
					}
				}
			}
		}

		switch ((*it)->type)
		{
			case ymap:
			{
				(*it)->file->Finalize();
				break;
			}
			case ydr:
			case ydd:
			case yft:
			{
				(*it)->file->Init(stream, (*it)->SystemSize);
				GlobalGpuMemory += (*it)->file->gpuMemory;
				break;
			}
			case ytd:
			{
				(*it)->file->Init(stream, (*it)->SystemSize);
				TextureMemory += (*it)->file->gpuMemory;
				break;
			}
			case ybn:
			{
				(*it)->file->Finalize(dynamicsWorld);
				break;
			}
		}

		delete *it;
		it = resources.erase(it);
	}
	resources_lock.unlock();
}

void GameWorld::createPedestrian()
{
	//Player *newPlayer = myNew Player(glm::vec3(0, 20, 0), dynamicsWorld);
	//pedestrians.push_back(newPlayer);
}

void GameWorld::createVehicle(glm::vec3 position)
{
	int vehicleID = rand() % vehiclesPool.size();

	auto it = vehiclesPool.begin();
	std::advance(it, vehicleID);
	if (it->second.file == nullptr) {
		it->second.file = GetYft(it->first, it->first);
	}
	else {
		Vehicle* veh = myNew Vehicle(position, it->second.mass, it->second.file, dynamicsWorld);
		vehicles.push_back(veh);
	}
}

void GameWorld::UpdateDynamicObjects()
{
	for (auto & map : CurYmaps)
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
	}
}

void GameWorld::Update()
{
	for (auto& pedestrian : pedestrians)
	{
		pedestrian.PhysicsTick();
	}

	for (auto& vehicle : vehicles)
	{
		vehicle->PhysicsTick();
	}
}

void GameWorld::UpdateTraffic(Camera* camera, glm::vec3 pos)
{
	float radiusTraffic = 5.0f;
	//PEDESTRIANS

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
				glm::vec3 vehiclePosition(vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getX(), vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getY(), vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getZ());
				if (glm::distance(camera->position, vehiclePosition) >= 100.0f) {
					dynamicsWorld->removeVehicle((vehicles[i]->m_vehicle));
					dynamicsWorld->removeRigidBody((vehicles[i]->m_carChassis));
					delete vehicles[i];
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

Vehicle* GameWorld::FindNearestVehicle()
{
	float d = 15.0f;

	Vehicle* nearestVehicle = nullptr;

	for (auto& vehicle : vehicles)
	{
		glm::vec3 PlayerPosition(pedestrians[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getX(), pedestrians[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getY(), pedestrians[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getZ());
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

constexpr float deltaTime = 1.f / 120.f;

void GameWorld::update(float delta_time, Camera* camera)
{
	dynamicsWorld->stepSimulation(delta_time);

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
