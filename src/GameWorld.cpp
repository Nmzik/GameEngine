#include "GameWorld.h"

#include "GTAEncryption.h"
#include "ResourceManager.h"
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

GameWorld::GameWorld() :
	dirLight(glm::vec3(0.1f, 0.8f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), true)
	{
	//////////////////////////////////////////////////////////////////////////////

	resourceManager = std::make_unique<ResourceManager>(this);
	/*{
	 std::vector<YdrLoader*> ydrs;
	 ydrs.reserve(1500);

	 uint32_t valuetest = 0;

	 for (auto it = data.YdrEntries.begin(); valuetest < 1500; it++)
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
	 glFinish();
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
	}*/

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
		if (data.cacheFile->AllMapNodes[i].Unk1 == 1) //	NOT SURE
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

	//	RenderList
	renderList.reserve(2000);

	resourceManager->GetYdr(2096445108);
	resourceManager->GetYtd(3403519606);

	for (auto& WaterQuad : data.WaterQuads)
	{
		Water water(WaterQuad);
		WaterMeshes.push_back(water);
	}

	gameHour = 10;
	gameMinute = 0;

	resourceManager->GetYtd(4096714883); //PLAYER YTD
	YddLoader* playerYDD = resourceManager->GetYdd(4096714883);
	
	skydome = resourceManager->GetYdd(2640562617);

	resourceManager->GetYtd(GenHash(std::string("mapdetail")));
	resourceManager->GetYtd(GenHash(std::string("vehshare")));
	resourceManager->GetYtd(GenHash(std::string("vehshare_worn")));
	resourceManager->GetYtd(GenHash(std::string("vehshare_army")));
	resourceManager->GetYtd(GenHash(std::string("vehshare_truck")));

	/*for (auto& ytd : data.GtxdEntries)
	{
		resourceManager->GetYtd(ytd.second);
	}*/

	for (auto& vehicle : vehiclesPool)
	{
		resourceManager->GetYtd(vehicle.first);
	}

	while (!skydome->Loaded || !playerYDD->Loaded)
	{
		LoadQueuedResources();
	}

	peds.emplace_back(glm::vec3(-2020.47, -449.58, 19.71), playerYDD);
	peds.emplace_back(glm::vec3(9.66, -1184.98, 75.74), playerYDD);
	peds.emplace_back(glm::vec3(2250.18f, 3471.40f, 56.50f), playerYDD);

	vehicles.reserve(50);
}

GameWorld::~GameWorld()
{

}

void GameWorld::LoadYmap(YmapLoader* map, Camera* camera, glm::vec3& position)
{
	if (map->Loaded)
	{
		for (auto& object : map->Objects)
		{
			float Dist = glm::length2(position - object.position);
			bool IsVisible = Dist <= object.CEntity.lodDist * LODMultiplier;
			bool childrenVisible = (Dist <= object.CEntity.childLodDist * LODMultiplier) && (object.CEntity.numChildren > 0);
			if (IsVisible && !childrenVisible)
			{
				if (!object.Loaded)
				{
					switch (object.archetype->BaseArchetypeDef.assetType)
					{
						case ASSET_TYPE_DRAWABLE:
						{
							if (!object.FoundModel)
							{
								object.ytd = resourceManager->GetYtd(object.archetype->BaseArchetypeDef.textureDictionary);
								object.ydr = resourceManager->GetYdr(object.CEntity.archetypeName);
								object.FoundModel = true;
							}
							if (object.ydr->Loaded)
							{
								//	NOTE:
								//
								//	SPAWN OBJECTS STATICALLY (IN SLEEP STATE)
								//
								//	SUPER DIRTY NEED FIX URGENT! UGLY FIX!!!
								/*if (object.ydr->ybnfile) {

								 if (object.ydr->ybnfile->compound->getNumChildShapes() != 0) {

								  //SET POSITION OF COLLISION TO OBJECT PLACE
								  //NOTE: SPAWN AT EXACT SAME PLACE!!! "NO +1.0f"
								  btVector3 localInertia(0, 0, 0);
								  float mass = 0.0f;

								  btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(-object.rotation.x, object.rotation.y, object.rotation.z, object.rotation.w),
								btVector3(object.position.x, object.position.y, object.position.z))); btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, MotionState, object.ydr->ybnfile->compound, localInertia);
								  object.rigidBody = new btRigidBody(groundRigidBodyCI);
								  dynamicsWorld->addRigidBody(object.rigidBody);
								 }//can be an error here
								}*/

								object.Loaded = true;
							}
							break;
						}
						case ASSET_TYPE_DRAWABLEDICTIONARY:
						{
							if (!object.FoundModel)
							{
								object.ytd = resourceManager->GetYtd(object.archetype->BaseArchetypeDef.textureDictionary);
								object.ydd = resourceManager->GetYdd(object.archetype->BaseArchetypeDef.drawableDictionary);
								object.FoundModel = true;
							}
							if (object.ydd->Loaded)
							{
								auto iter2 = object.ydd->YdrFiles.find(object.CEntity.archetypeName);
								if (iter2 != object.ydd->YdrFiles.end())
								{
									object.ydr = iter2->second;
									object.Loaded = true;
								}
							}
							break;
						}
						case ASSET_TYPE_FRAGMENT:
						{
							if (!object.FoundModel)
							{
								object.ytd = resourceManager->GetYtd(object.archetype->BaseArchetypeDef.textureDictionary);
								object.yft = resourceManager->GetYft(object.CEntity.archetypeName);
								object.FoundModel = true;
							}
							if (object.yft->Loaded)
							{
								object.ydr = object.yft->ydr;

								

								object.Loaded = true;
							}
							break;
						}
					}
				}
				else
				{

					if ((object.archetype->BaseArchetypeDef.flags & 2048) > 0)
					{
						//	if (!renderProxies) continue;
						continue;
					}

					if (object.archetype->GetType() == 1)
					{ //	TIME ARCHETYPE
						//	if ((object.Archetype._TimeArchetypeDef.timeFlags >> gameHour) & 1)
						//{
						continue;
						//}
					}

					if (camera->intersects(object.BoundPos, object.BoundRadius))
					{
						renderList.push_back(&object);
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
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position) * glm::mat4_cast(glm::quat(-map->CMloInstanceDefs[i].fwEntityDef.rotation.w,
	-map->CMloInstanceDefs[i].fwEntityDef.rotation.x, -map->CMloInstanceDefs[i].fwEntityDef.rotation.y, -map->CMloInstanceDefs[i].fwEntityDef.rotation.z)) * glm::scale(glm::mat4(1.0f),
	glm::vec3(EntityDef.scaleXY, EntityDef.scaleXY, EntityDef.scaleZ));

		LoadYDR(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position, 1.0f, matrix);
		//LoadYDD(camera, map->CMloInstanceDefs[i].fwEntityDef.archetypeName, 0, it->second._BaseArchetypeDef.bsCentre + map->fwEntityDefs[i].position, it->second._BaseArchetypeDef.bsRadius *
	std::max(map->fwEntityDefs[i].scaleXY, map->fwEntityDefs[i].scaleZ), it->second._BaseArchetypeDef.drawableDictionary, map->ModelMatrices[i]);
		//LoadYFT(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position, 100.0f, matrix);
	   }
	  }
	 }
	}*/

	//	NOTE::
	//
	//	RANDOM VEHICLE REALLY SPAWNS in RANDOM PLACE on the same CCarGens IN real game (spawns/despawns based on distance to player, checked on the same place)
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
	//glm::vec3 PlayerPos = peds[currentPlayerID].getPosition();
		glm::vec3 PlayerPos = camera->position;

	auto cellID = spaceGrid.GetCellPos(PlayerPos);
	auto NodeCell = nodeGrid.GetCellPos(PlayerPos);

	if (CurNodeCell != NodeCell)
	{
		CurNodeCell = NodeCell;
	}

	if (CurCell != cellID)
	{
		//	printf("NEW CELL\n");
		CurCell = cellID;

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
		//	Clear previous Ybns

		SpaceGridCell& cell = spaceGrid.GetCell(cellID);

		/*for (auto& BoundsItem : cell.BoundsStoreItems)
		{
			CurYbns.emplace_back(resourceManager->GetYbn(data.cacheFile->AllBoundsStoreItems[BoundsItem].Name));
		}*/

		for (auto& mapNode : cell.MapNodes)
		{
			CurYmaps.emplace_back(resourceManager->GetYmap(data.cacheFile->AllMapNodes[mapNode].Name));
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

	//	printf("FREE VAO %zd\n",MeshManager::VAOs.size());
	//	printf("FREE VBO %zd\n",MeshManager::VBOs.size());
	//	printf("FREE Textures %zd\n", TextureManager::TexturesID.size());
	//	printf("SIZE OBJECTS %zd\n",renderList.size());

	/*glm::i32vec2 test = nodeGrid.GetCellPos(camera->position);

	if (nodeGrid.cells[test.x * 32 + test.y]->ynd) {

	 for (auto& node : nodeGrid.cells[test.x * 32 + test.y]->ynd->nodes)
	 {
	  //printf("FOUND %f %f %f\n", node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f);
	  UpdateTraffic(camera, glm::vec3(node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f));

	 }
	}*/

	/*for (auto& ped : peds)
	{
	 //if ped loaded
	 auto& model = ped.getPosition();
	 if (camera->intersects(ped.position, 1.0f)) {

	  for (auto& ydr : ped.playerModel)
	  {
	   renderList.push_back(ydr);
	  }
	 }
	}*/

	std::sort(renderList.begin(), renderList.end(), [&camera](Object* a, Object* b) { // FRONT_TO_BACK
		glm::vec3 lhsPosition = glm::vec3(a->modelMatrix[3]);
		glm::vec3 rhsPosition = glm::vec3(b->modelMatrix[3]);

		return glm::distance2(lhsPosition, camera->position) < glm::distance2(rhsPosition, camera->position);
	});

	//	printf("CULLED :%d\n", ydrLoader.size());
	//	culled = 0;

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

	resourceManager->UpdateResourceCache();

	LoadQueuedResources();
}

void GameWorld::LoadQueuedResources()
{
	resources_lock.lock();
	if (resources.size() > 0)
		resourcesThread.swap(resources);
	resources_lock.unlock();

	//std::chrono::duration< double > fs(0);
	//HASH 38759883
	//auto old_time = std::chrono::steady_clock::now();
	//&& fs.count() < 0.003
	while (resourcesThread.size() > 0)
	{
		Resource* res = resourcesThread.front();
		resourcesThread.pop_front();

		//	Object hash equal to texture hash what should we do? there are +hi textures with the same name

		if (res->Buffer.size() == 0)
		{
			res->file->Loaded = true;
		}
		else
		{
			memstream stream(res->Buffer.data(), res->Buffer.size());
			stream.systemSize = res->SystemSize;
			switch (res->type)
			{
				case ymap:
				{
					YmapLoader* iter = static_cast<YmapLoader*>(res->file);

					memstream stream(res->Buffer.data(), res->Buffer.size());
					iter->Init(stream);

					for (auto& object : iter->Objects)
					{
						std::unordered_map<uint32_t, Archetype*>::iterator it = getGameData()->Archetypes.find(object.CEntity.archetypeName);
						if (it != getGameData()->Archetypes.end())
						{
							object.archetype = it->second;

							object.BoundPos = object.CEntity.position - object.archetype->BaseArchetypeDef.bsCentre;
							object.BoundRadius = object.archetype->BaseArchetypeDef.bsRadius; //* std::max(object.CEntity.scaleXY, object.CEntity.scaleZ); TREES doesnt render with multiplying by scale

							if (object.CEntity.lodDist <= 0)
								object.CEntity.lodDist = it->second->BaseArchetypeDef.lodDist;
							if (object.CEntity.childLodDist <= 0)
								object.CEntity.childLodDist = it->second->BaseArchetypeDef.lodDist;
						}
						else
						{
							printf("ERROR\n");
						}

						object.CEntity.lodDist *= object.CEntity.lodDist;           // glm::length2
						object.CEntity.childLodDist *= object.CEntity.childLodDist; // glm::length2
					}
					res->file->Loaded = true;
					break;
				}
				case ydr:
				case ydd:
				case yft:
				{
					res->file->Init(stream);
					resourceManager->GlobalGpuMemory += res->file->gpuMemory;
					break;
				}
				case ytd:
				{
					res->file->Init(stream);
					resourceManager->TextureMemory += res->file->gpuMemory;
					break;
				}
				case ybn:
				{
					YbnLoader* ybn = static_cast<YbnLoader*>(res->file);
					ybn->Init(stream);
					ybn->Finalize();
					break;
				}
			}
		}

		delete res;

		//auto current_time = std::chrono::steady_clock::now();
		//fs = current_time - old_time;
	}
}

void GameWorld::createPedestrian()
{
	//	Player *newPlayer = myNew Player(glm::vec3(0, 20, 0), dynamicsWorld);
	//	peds.push_back(newPlayer);
}

void GameWorld::createVehicle(glm::vec3 position)
{
	int vehicleID = rand() % vehiclesPool.size();

	auto it = vehiclesPool.begin();
	std::advance(it, vehicleID);
	if (it->second.file == nullptr)
	{
		it->second.file = resourceManager->GetYft(it->first);
		//	YTD???
	}
	else
	{
		if (it->second.file->Loaded) {
			CVehicle* veh = new CVehicle(position, it->second.mass, it->second.file);
			vehicles.emplace_back(veh);
		}
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

void GameWorld::UpdateTraffic(Camera* camera, glm::vec3 pos)
{
	float radiusTraffic = 20.0f;
	//	peds

	for (int i = 0; i < vehicles.size(); i++)
	{
		glm::vec3 vehiclePosition(vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getX(), vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getY(),
			vehicles[i]->m_carChassis->getWorldTransform().getOrigin().getZ());
		if (glm::distance(pos, vehiclePosition) >= 100.0f)
		{
			delete vehicles[i];
			vehicles.erase(vehicles.begin() + i);
		}
	}

	uint64_t MaximumAvailableVehicles = 30 - vehicles.size(); //	HARDCODED
	if (pos.z < 100.0f)
	{
		for (uint32_t i = 0; i < MaximumAvailableVehicles; i++)
		{
			float xRandom = RandomFloat(pos.x - radiusTraffic, pos.x + radiusTraffic);
			float yRandom = RandomFloat(pos.y - radiusTraffic, pos.y + radiusTraffic);
			if (!camera->intersects(glm::vec3(xRandom, yRandom, pos.z), 1.0f))
			{
				createVehicle(glm::vec3(xRandom, yRandom, pos.z));
			}
		}
	}

	/*if (pedPool.firstAvailable_ == nullptr) {
	 for (auto& ped : pedPool.peds)
	 {
	  if (glm::distance(camera->position, glm::vec3(ped.getPhysCharacter()->getWorldTransform().getOrigin().getX(), ped.getPhysCharacter()->getWorldTransform().getOrigin().getY(),
	ped.getPhysCharacter()->getWorldTransform().getOrigin().getZ())) >= 100.0f) { pedPool.Remove(&ped);
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
	//	CARS
	/*if (nodeGrid.cells[CurNodeCell.x * 32 + CurNodeCell.y]->ynd)
	{

		for (auto& node : nodeGrid.cells[CurNodeCell.x * 32 + CurNodeCell.y]->ynd->nodes)
		{
			pos = glm::vec3(node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f);

			for (int i = 0; i < vehicles.size(); i++)
			{
				glm::vec3 vehiclePosition(vehicles[i].m_carChassis->getWorldTransform().getOrigin().getX(), vehicles[i].m_carChassis->getWorldTransform().getOrigin().getY(),
					vehicles[i].m_carChassis->getWorldTransform().getOrigin().getZ());
				if (glm::distance(camera->position, vehiclePosition) >= 100.0f)
				{
					dynamicsWorld->removeVehicle((vehicles[i].m_vehicle));
					dynamicsWorld->removeRigidBody((vehicles[i].m_carChassis));
					vehicles.erase(vehicles.begin() + i);
				}
			}
			uint64_t MaximumAvailableVehicles = 30 - vehicles.size(); //	HARDCODED
			if (camera->position.z < 100.0f)
			{
				for (uint32_t i = 0; i < MaximumAvailableVehicles; i++)
				{
					float xRandom = RandomFloat(pos.x - radiusTraffic, pos.x + radiusTraffic);
					float yRandom = RandomFloat(pos.y - radiusTraffic, pos.y + radiusTraffic);
					if (!camera->intersects(glm::vec3(xRandom, yRandom, pos.z), 1.0f))
					{
						createVehicle(glm::vec3(xRandom, yRandom, pos.z));
					}
				}
			}
		}
	}*/
}

CVehicle* GameWorld::FindNearestVehicle()
{
	float d = 15.0f;

	CVehicle* nearestVehicle = nullptr;

	for (auto& vehicle : vehicles)
	{
		glm::vec3 PlayerPosition(peds[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getX(), peds[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getY(),
			peds[currentPlayerID].getPhysCharacter()->getWorldTransform().getOrigin().getZ());
		glm::vec3 VehiclePosition(vehicle->m_carChassis->getWorldTransform().getOrigin().getX(), vehicle->m_carChassis->getWorldTransform().getOrigin().getY(), vehicle->m_carChassis->getWorldTransform().getOrigin().getZ());
		float vd = glm::length(PlayerPosition - VehiclePosition);
		if (vd < d)
		{
			d = vd;
			nearestVehicle = vehicle;
		}
	}

	return nearestVehicle;
}

void GameWorld::DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection)
{
	glm::vec3 HitPos = CameraPosition + lookDirection;
	btVector3 from(CameraPosition.x, CameraPosition.y, CameraPosition.z), to(HitPos.x, HitPos.y, HitPos.z);
	btCollisionWorld::ClosestRayResultCallback cb(from, to);
	cb.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	PhysicsSystem::dynamicsWorld->rayTest(from, to, cb);

	if (cb.hasHit())
	{

		CPed* player = static_cast<CPed*>(cb.m_collisionObject->getUserPointer());
		//	printf("player pointer %p\n", (void*)&player);
		if (player != nullptr)
		{
			player->TakeDamage(20);
			//	player->getPhysCharacter()->applyImpulse(btVector3(0.0f, 0.0f, -20.0f));
			if (!player->isAlive())
			{
				printf("KILLED!\n");
				//	fun stuff
				//	player->Jump();
				//	ACTUALLY DONT DELETE PEDESTRIAN/PLAYER (MESH SHOULD BE STAYED ) - for example ambulance coming to help him etc
				//	DELETEING WILL BE DONE BY TRAFFIC UPDATER (CAMERA FAR FROM PEDESTRIAN -> DELETE!)
			}
		}
		//	cb.setUserPointer
	}
}

constexpr float myOWNdeltaTime = 1.f / 120.f;

void GameWorld::update(float delta_time, Camera* camera)
{
	physicsSystem.Update(delta_time);

	static float clockAccumulator = 0.f;

	clockAccumulator += delta_time;
	while (clockAccumulator >= 1.f) {
		gameMinute++;
		while (gameMinute >= 60) {
			gameMinute = 0;
			gameHour++;
			while (gameHour >= 24) {
				gameHour = 0;
			}
		}
		clockAccumulator -= 1.f;
	}

	//	printf("Time %d %d\n", getWorld()->gameHour, getWorld()->gameMinute);


	double SUNRISE = 5.47f; // % of Day
	double SUNSET = 19.35f;
	uint8_t MOONRISE = 17; // % of Day
	uint8_t MOONSET = 4;
	float tod = gameHour + gameMinute / 60.f;
	if (tod > SUNRISE && tod < SUNSET)
	{
		double sunT = ((double)tod - SUNRISE) / (SUNSET - SUNRISE);
		float phi = glm::pi<float>() / 2.0f - (float)sunT * glm::pi<float>();
		float theta = 0.0f;

		dirLight.direction = glm::normalize(glm::vec3(-glm::sin(phi) * glm::cos(theta), glm::sin(phi) * glm::sin(theta), glm::cos(phi)));
	}

	//float now = (timeOfDay / 24) * glm::two_pi<float>() + glm::pi<float>();

	//glm::vec3 sunDirection(0, cos(now), sin(now));
	//sunDirection = glm::normalize(sunDirection);

	UpdateDynamicObjects();
	//UpdateTraffic(camera, peds[currentPlayerID].getPosition());
	for (auto& pedestrian : peds)
	{
		pedestrian.PhysicsTick();
	}

	for (auto& vehicle : vehicles)
	{
		vehicle->PhysicsTick();
	}
}

bool GameWorld::DetectInWater(glm::vec3 Position)
{
	for (auto& waterQuad : data.WaterQuads)
	{
		if (waterQuad.minX < Position.x && waterQuad.maxX > Position.x && waterQuad.minY < Position.y && waterQuad.maxY > Position.y)
		{
			//	printf("WE ARE IN WATER\n");
			return true;
		}
	}
	return false;
}
	
void GameWorld::TestFunction(glm::vec3 Position)
{
	for (auto& ytd : data.GtxdEntries)
	{
		while (!resourceManager->GetYtd(ytd.second))
		{
			LoadQueuedResources();
		}
	}
	printf("DONE\n");
}

void GameWorld::ClearTestFunction()
{
	resourceManager->RemoveAll();
}
