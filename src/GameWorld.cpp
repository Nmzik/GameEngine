#include "GameWorld.h"
#include "common.h"

#include "CBuilding.h"
#include "Camera.h"
#include "GTAEncryption.h"
#include "GameData.h"
#include "ResourceManager.h"
#include "YbnLoader.h"
#include "YddLoader.h"
#include "YftLoader.h"
#include "YmapLoader.h"
#include "YndLoader.h"
#include "YnvLoader.h"
#include "YtdLoader.h"
#include "YtypLoader.h"

GameWorld::GameWorld(GameData* _gameData)
    : dirLight(glm::vec3(0.1f, 0.8f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), true)
    , data(*_gameData)
    , resourceManager(std::make_unique<ResourceManager>(this))
{
    /*for (auto& vehicle : data.VehiclesInfo)
    {
        vehiclesPool[vehicle.Hash] = vehicle;
    }*/

    CurYbns.reserve(50);
    CurYmaps.reserve(200);  //> 100
    vehicles.reserve(50);
    peds.reserve(20);
    //	RenderList
    renderList.reserve(2000);

    /*WaterMeshes.reserve(data.WaterQuads.size());
    for (auto& WaterQuad : data.WaterQuads)
    {
        Water water(WaterQuad);
        WaterMeshes.push_back(water);
    }*/

    gameHour = 10;
    gameMinute = 0;

    resourceManager->GetYtd(3403519606);  //water
    resourceManager->GetYtd(4096714883);  //PLAYER YTD

    YddLoader* playerYDD = resourceManager->GetYdd(4096714883);
    skydome = resourceManager->GetYdd(2640562617);

    while (!skydome->Loaded || !playerYDD->Loaded)
    {
        loadQueuedResources();
    }

    resourceManager->GetYtd(GenHash("mapdetail"));
    resourceManager->GetYtd(GenHash("vehshare"));
    resourceManager->GetYtd(GenHash("vehshare_worn"));
    resourceManager->GetYtd(GenHash("vehshare_army"));
    resourceManager->GetYtd(GenHash("vehshare_truck"));

    /*for (auto& ytd : data.GtxdEntries)
	{
	 resourceManager->GetYtd(ytd.second);
	}*/

    AddPedToWorld(glm::vec3(461.585052, -1314.648804, 29.332735), playerYDD);
    //AddPedToWorld(glm::vec3(1705.95, 3746.39, 37.64), playerYDD);
    AddPedToWorld(glm::vec3(9.66, -1184.98, 75.74), playerYDD);
    AddPedToWorld(glm::vec3(2250.18f, 3471.40f, 56.50f), playerYDD);

    //minimap
    std::vector<YddLoader*> minimapsYDD;
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_0_2")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_0_3")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_0_4")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_0_5")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_0_6")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_1")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_2")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_3")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_4")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_5")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_6")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_7")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_1_8")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_0")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_1")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_2")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_3")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_4")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_5")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_6")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_7")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_2_8")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_0")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_1")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_2")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_3")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_4")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_5")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_6")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_7")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_3_8")));
    minimapsYDD.push_back(resourceManager->GetYdd(GenHash("minimap_4_0")));

    for (int i = 0; i < minimapsYDD.size(); i++)
    {
        while (!minimapsYDD[i]->Loaded)
        {
            loadQueuedResources();
        }
    }

    fwEntityDef def;
    def.position = glm::vec3(0.f, 0.f, -100.f);
    def.rotation = glm::quat(0.f, 0.f, 0.f, -1.f);
    def.scaleXY = 1.0f;
    def.scaleZ = 1.0f;

    for (int i = 0; i < minimapsYDD.size(); i++)
    {
        for (auto& ydr : minimapsYDD[i]->ydrFiles)
        {
            CBuilding building(def);
            building.ydr = ydr.second.get();
            minimap.push_back(building);
        }
    }
}

GameWorld::~GameWorld()
{
}

void GameWorld::updateObjects(Camera* camera, glm::vec3& position)
{
    culledYmaps = 0;
    for (auto& ymap : CurYmaps)
    {
        if (ymap->Loaded)
        {
            if (!camera->ContainsAABBNoClipNoOpt(ymap->_CMapData.entitiesExtentsMin, ymap->_CMapData.entitiesExtentsMax))
            {
                ++culledYmaps;
                continue;
            }
            for (auto& object : ymap->Objects)
            {
                float Dist = glm::length2(position - object.getPosition());
                bool IsVisible = Dist <= object.EntityDef.lodDist * LODMultiplier;
                bool childrenVisible = (Dist <= object.EntityDef.childLodDist * LODMultiplier) && (object.EntityDef.numChildren > 0);
                if (IsVisible && !childrenVisible)
                {
                    if (!object.Loaded)
                    {
                        switch (object.archetype->BaseArchetypeDef.assetType)
                        {
                            case ASSET_TYPE_DRAWABLE:
                            {
                                if (!object.ydr)
                                {
                                    object.ytd = resourceManager->GetYtd(object.archetype->BaseArchetypeDef.textureDictionary);
                                    object.ydr = resourceManager->GetYdr(object.EntityDef.archetypeName);
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

							  btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(-object.rotation.x, object.rotation.y,
							object.rotation.z, object.rotation.w), btVector3(object.position.x, object.position.y, object.position.z)));
							btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, MotionState, object.ydr->ybnfile->compound, localInertia); object.rigidBody =
							new btRigidBody(groundRigidBodyCI); dynamicsWorld->addRigidBody(object.rigidBody);
							 }//can be an error here
							}*/

                                    object.Loaded = true;
                                }
                                break;
                            }
                            case ASSET_TYPE_DRAWABLEDICTIONARY:
                            {
                                if (!object.ydd)
                                {
                                    object.ytd = resourceManager->GetYtd(object.archetype->BaseArchetypeDef.textureDictionary);
                                    object.ydd = resourceManager->GetYdd(object.archetype->BaseArchetypeDef.drawableDictionary);
                                }
                                if (object.ydd->Loaded)
                                {
                                    auto iter2 = object.ydd->ydrFiles.find(object.EntityDef.archetypeName);
                                    if (iter2 != object.ydd->ydrFiles.end())
                                    {
                                        object.ydr = iter2->second.get();
                                        object.Loaded = true;
                                    }
                                }
                                break;
                            }
                            case ASSET_TYPE_FRAGMENT:
                            {
                                if (!object.yft)
                                {
                                    object.ytd = resourceManager->GetYtd(object.archetype->BaseArchetypeDef.textureDictionary);
                                    object.yft = resourceManager->GetYft(object.EntityDef.archetypeName);
                                }
                                if (object.yft->Loaded)
                                {
                                    object.ydr = object.yft->ydr.get();
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

                        bool isreflproxy = false;
                        switch (object.EntityDef.flags)
                        {
                            case 135790592:  //	001000000110000000000000000000    prewater proxy (golf course)
                            case 135790593:  //	001000000110000000000000000001    water refl proxy? (mike house)
                            case 672661504:  //	101000000110000000000000000000    vb_ca_prop_tree_reflprox_2
                            case 536870912:  //	100000000000000000000000000000    vb_05_emissive_mirroronly
                            case 35127296:   //	000010000110000000000000000000    tunnel refl proxy?
                            case 39321602:   //	000010010110000000000000000010    mlo reflection?
                                isreflproxy = true;
                                break;
                        }
                        if (isreflproxy)
                        {
                            continue;
                        }

                        if (object.archetype->GetType() == 1)
                        {  //	TIME ARCHETYPE
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
    }

    glm::vec3 camPosition = camera->getPosition();
    for (auto& ped : peds)
    {
        renderList.push_back(ped);
    }

    for (auto& vehicle : vehicles)
    {
        renderList.push_back(vehicle);
    }

    for (int i = 0; i < minimap.size(); i++)
    {
        renderList.push_back(&minimap[i]);
    }

    std::sort(renderList.begin(), renderList.end(), [&camPosition](CEntity* a, CEntity* b) {  // FRONT_TO_BACK
        glm::vec3 lhsPosition = glm::vec3(a->getMatrix()[3]);
        glm::vec3 rhsPosition = glm::vec3(b->getMatrix()[3]);

        return glm::distance2(lhsPosition, camPosition) < glm::distance2(rhsPosition, camPosition);
    });

    /*if (map->CMloInstanceDefs.size() > 0) {
	 for (int i = 0; i < map->CMloInstanceDefs.size(); i++)
	 {
	  auto it = data.MloDictionary.find(map->CMloInstanceDefs[i].fwEntityDef.archetypeName);
	  if (it != data.MloDictionary.end())
	  {
	   for (auto& EntityDef : it->second)
	   {
	 glm::vec4 rotmultiply = EntityDef.rotation * map->CMloInstanceDefs[i].fwEntityDef.rotation;
	 glm::mat4 matrix = glm::translate(glm::mat4(1.0f), map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position) *
	glm::mat4_cast(glm::quat(-map->CMloInstanceDefs[i].fwEntityDef.rotation.w, -map->CMloInstanceDefs[i].fwEntityDef.rotation.x,
	-map->CMloInstanceDefs[i].fwEntityDef.rotation.y, -map->CMloInstanceDefs[i].fwEntityDef.rotation.z)) * glm::scale(glm::mat4(1.0f),
	glm::vec3(EntityDef.scaleXY, EntityDef.scaleXY, EntityDef.scaleZ));

	 LoadYDR(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position, 1.0f, matrix);
	 //LoadYDD(camera, map->CMloInstanceDefs[i].fwEntityDef.archetypeName, 0, it->second._BaseArchetypeDef.bsCentre + map->fwEntityDefs[i].position,
	it->second._BaseArchetypeDef.bsRadius * std::max(map->fwEntityDefs[i].scaleXY, map->fwEntityDefs[i].scaleZ),
	it->second._BaseArchetypeDef.drawableDictionary, map->ModelMatrices[i]);
	 //LoadYFT(camera, EntityDef.archetypeName, 0, map->CMloInstanceDefs[i].fwEntityDef.position + EntityDef.position, 100.0f, matrix);
	   }
	  }
	 }
	}*/

    //	NOTE::
    //
    //	RANDOM VEHICLE spawns in RANDOM PLACE on the same CCarGens IN real game (spawns/despawns based on distance to player, checked on the same place)
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

  YtypLoader* file = new YtypLoader(stream);
  ytypLoader.push_back(file);

  return true;
 }
}*/

void GameWorld::getVisibleYmaps(glm::vec3& PlayerPos)
{
    auto cellID = spaceGrid.GetCellPos(PlayerPos);
    auto NodeCell = nodeGrid.GetCellPos(PlayerPos);
    auto NavCell = navGrid.GetCellPos(PlayerPos);

    if (CurNavCell != NavCell)
    {
        CurNavCell = NavCell;

        //auto& cell = navGrid.GetCell(NavCell);
    }

    if (CurNodeCell != NodeCell)
    {
        CurNodeCell = NodeCell;

        //auto& cell = nodeGrid.GetCell(NodeCell);
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

        //SpaceGridCell& cell = spaceGrid.GetCell(cellID);

        //	Bounds
        for (int i = 0; i < data.cacheFile->AllBoundsStoreItems.size(); i++)
        {
            glm::i32vec2 min = spaceGrid.GetCellPos(data.cacheFile->AllBoundsStoreItems[i].Min);
            glm::i32vec2 max = spaceGrid.GetCellPos(data.cacheFile->AllBoundsStoreItems[i].Max);

            if (cellID.x <= max.x && cellID.x >= min.x && cellID.y <= max.y && cellID.y >= min.y)
            {
                CurYbns.emplace_back(resourceManager->GetYbn(data.cacheFile->AllBoundsStoreItems[i].Name));
            }
        }

        for (int i = 0; i < data.cacheFile->AllMapNodes.size(); i++)
        {
            glm::i32vec2 min = spaceGrid.GetCellPos(data.cacheFile->AllMapNodes[i].streamingExtentsMin);  //	NOT entitiesExtents !!!! GIVES WEIRD RESULTS
            glm::i32vec2 max = spaceGrid.GetCellPos(data.cacheFile->AllMapNodes[i].streamingExtentsMax);

            if (cellID.x <= max.x && cellID.x >= min.x && cellID.y <= max.y && cellID.y >= min.y)
            {
                CurYmaps.emplace_back(resourceManager->GetYmap(data.cacheFile->AllMapNodes[i].Name));
            }
        }
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

    loadQueuedResources();
}

void GameWorld::loadQueuedResources()
{
    //	If we still didn't finish loading our queue, do not swap! Swap only if we dont have any job.
    if (resourcesThread.size() == 0)
    {
        resources_lock.lock();
        if (resources.size() > 0)
            resourcesThread.swap(resources);
        resources_lock.unlock();
    }

    //	HASH 38759883

    auto old_time = std::chrono::steady_clock::now();

    long long diffms = 0;

    while (resourcesThread.size() > 0 && diffms < 2)  //	2ms
    {
        Resource* res = resourcesThread.front();
        resourcesThread.pop_front();

        //	Object hash equal to texture hash what should we do? there are +hi textures with the same name

        if (res->BufferSize == 0)
        {
            res->file->Loaded = true;
        }
        else
        {
            memstream stream(&res->Buffer[0], res->BufferSize);
            stream.systemSize = res->SystemSize;
            switch (res->type)
            {
                case ymap:
                {
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
                    getPhysicsSystem()->addRigidBody(ybn->getRigidBody());  //	NOT THREAD SAFE!
                    break;
                }
                case ysc:
                {
                    res->file->Init(stream);
                    break;
                }
            }

            resourceManager->resource_allocator->deallocate(res->Buffer);
        }

        delete res;

        auto new_time = std::chrono::steady_clock::now();
        diffms = std::chrono::duration_cast<std::chrono::microseconds>(new_time - old_time).count();
    }
}

void GameWorld::createPedestrian()
{
    //	Player *newPlayer = new Player(glm::vec3(0, 20, 0), dynamicsWorld);
    //	peds.push_back(newPlayer);
}

void GameWorld::createVehicle(glm::vec3 position)
{
    int vehicleID = rand() % data.VehiclesInfo.size();

    if (YftLoader* vehicle = resourceManager->GetYft(data.VehiclesInfo[vehicleID].Hash); vehicle->Loaded)
    {
        AddVehicleToWorld(position, data.VehiclesInfo[vehicleID].mass, vehicle);
        printf("Car Spawned\n");
    }
}

void GameWorld::cleanupTraffic(Camera* camera)
{
    float radiusTraffic = 120.0f;
    //	peds
    /*for (auto it = peds.begin() + 3; it != peds.end();)
    {
        if (glm::distance(camera->getPosition(), (*it)->getPosition()) >= radiusTraffic)
        {
            RemovePedFromWorld(*it);
            delete *it;
            peds.erase(it);
        }
        else
        {
            ++it;
        }
    }*/
    //vehicles
    for (int i = 0; i < vehicles.size(); i++)
    {
        if (glm::distance(camera->getPosition(), vehicles[i]->getPosition()) >= radiusTraffic)
        {
            RemoveVehicleFromWorld(vehicles[i]);
            delete vehicles[i];
            printf("Car Removed\n");

            // ensure that we're not attempting to access out of the bounds of the container.
            assert(i < vehicles.size());

            //Swap the element with the back element, except in the case when we're the last element.
            if (i + 1 != vehicles.size())
                std::swap(vehicles[i], vehicles.back());

            //Pop the back of the container, deleting our old element.
            vehicles.pop_back();
        }
    }
}

void GameWorld::createTraffic(Camera* camera)
{
    float radiusTraffic = 100.0f;

    int maxSpawn = 10;

    const auto MaximumAvailableVehicles = maxSpawn - vehicles.size();

    for (auto i = 0; i < MaximumAvailableVehicles; i++)
    {
        glm::vec3 camPosition = camera->getPosition();
        float xRandom = RandomFloat(camPosition.x - radiusTraffic, camPosition.x + radiusTraffic);
        float yRandom = RandomFloat(camPosition.y - radiusTraffic, camPosition.y + radiusTraffic);
        if (!camera->intersects(glm::vec3(xRandom, yRandom, camPosition.z), 1.0f))
        {
            createVehicle(glm::vec3(xRandom, yRandom, camPosition.z));
        }
    }
}

void GameWorld::updateDynamicObjects()
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

/*void GameWorld::UpdateTraffic(Camera* camera, glm::vec3 pos)
{
//	CARS
/*if (nodeGrid.cells[CurNodeCell.x * 32 + CurNodeCell.y]->ynd)
	{

	 for (auto& node : nodeGrid.cells[CurNodeCell.x * 32 + CurNodeCell.y]->ynd->nodes)
	 {
	  pos = glm::vec3(node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f);

	  for (int i = 0; i < vehicles.size(); i++)
	  {
	   glm::vec3 vehiclePosition(vehicles[i].m_carChassis->getWorldTransform().getOrigin().getX(),
	vehicles[i].m_carChassis->getWorldTransform().getOrigin().getY(), vehicles[i].m_carChassis->getWorldTransform().getOrigin().getZ()); if
	(glm::distance(camera->position, vehiclePosition) >= 100.0f)
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
//}

CVehicle* GameWorld::findNearestVehicle()
{
    float d = 15.0f;

    CVehicle* nearestVehicle = nullptr;

    for (auto& vehicle : vehicles)
    {
        float vd = glm::length(peds[currentPlayerID]->getPosition() - vehicle->getPosition());
        if (vd < d)
        {
            d = vd;
            nearestVehicle = vehicle;
        }
    }

    return nearestVehicle;
}

void GameWorld::detectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection)
{
    glm::vec3 HitPos = CameraPosition + lookDirection;
    glm::vec3 from(CameraPosition.x, CameraPosition.y, CameraPosition.z), to(HitPos.x, HitPos.y, HitPos.z);
    auto result = physicsSystem.rayCast(from, to);

    if (result.HasHit)
    {
        /*CPed* player = static_cast<CPed*>(cb.m_collisionObject->getUserPointer());
        //	printf("player pointer %p\n", (void*)&player);
        if (player != nullptr)
        {
            player->takeDamage(20);
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
        //	cb.setUserPointer*/
    }
}

constexpr float myOWNdeltaTime = 1.f / 120.f;

void GameWorld::updateWorld(float delta_time, Camera* camera)
{
    physicsSystem.update(delta_time);

    static float clockAccumulator = 0.f;

    clockAccumulator += delta_time;
    while (clockAccumulator >= 1.f)
    {
        gameMinute++;
        while (gameMinute >= 60)
        {
            gameMinute = 0;
            gameHour++;
            while (gameHour >= 24)
            {
                gameHour = 0;
            }
        }
        clockAccumulator -= 1.f;
    }

    //	printf("Time %d %d\n", getWorld()->gameHour, getWorld()->gameMinute);

    double SUNRISE = 5.47f;  // % of Day
    double SUNSET = 19.35f;
    uint8_t MOONRISE = 17;  // % of Day
    uint8_t MOONSET = 4;
    float tod = gameHour + gameMinute / 60.f;
    if (tod > SUNRISE && tod < SUNSET)
    {
        double sunT = ((double)tod - SUNRISE) / (SUNSET - SUNRISE);
        float phi = glm::pi<float>() / 2.0f - (float)sunT * glm::pi<float>();
        float theta = 0.0f;

        dirLight.direction = glm::normalize(glm::vec3(-glm::sin(phi) * glm::cos(theta), glm::sin(phi) * glm::sin(theta), glm::cos(phi)));
    }

    //	float now = (timeOfDay / 24) * glm::two_pi<float>() + glm::pi<float>();

    //	glm::vec3 sunDirection(0, cos(now), sin(now));
    //	sunDirection = glm::normalize(sunDirection);

    //updateDynamicObjects();
    //cleanupTraffic(camera);
    //createTraffic(camera);

    for (auto& ped : peds)
    {
        ped->physicsTick();
    }

    for (auto& vehicle : vehicles)
    {
        vehicle->physicsTick();
    }

    if (EnableStreaming)
    {
        renderList.clear();

        glm::vec3 PlayerPos = getCurrentPlayer()->getPosition();
        //glm::vec3 PlayerPos = camera->position;

        getVisibleYmaps(PlayerPos);
        updateObjects(camera, PlayerPos);
    }
}

bool GameWorld::detectInWater(glm::vec3 Position)
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

void GameWorld::AddVehicleToWorld(glm::vec3 position, float mass, YftLoader* model)
{
    CVehicle* vehicle = new CVehicle(position, mass, model, physicsSystem.getPhysicsWorld());
    physicsSystem.addVehicle(vehicle);

    vehicles.push_back(vehicle);
}

void GameWorld::RemoveVehicleFromWorld(CVehicle* vehicle)
{
    physicsSystem.removeVehicle(vehicle);
}

void GameWorld::AddPedToWorld(glm::vec3 pos, YddLoader* model)
{
    CPed* ped = new CPed(pos, model);
    physicsSystem.addPed(ped);
    peds.push_back(ped);
}

void GameWorld::RemovePedFromWorld(CPed* ped)
{
    physicsSystem.removePed(ped);
}

void GameWorld::testFunction(glm::vec3 Position)
{
    /*for (auto& ytd : data.GtxdEntries)
	{
	 while (!resourceManager->GetYtd(ytd.second))
	 {
	  LoadQueuedResources();
	 }
	}
	printf("DONE\n");*/
}
