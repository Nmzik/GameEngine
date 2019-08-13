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
    , curNode(nullptr)
{
    /*for (auto& vehicle : data.VehiclesInfo)
    {
        vehiclesPool[vehicle.Hash] = vehicle;
    }*/

    curYbns.reserve(50);
    curYmaps.reserve(200);  //> 100
    vehicles.reserve(50);
    peds.reserve(20);
    //	RenderList
    renderList.reserve(2000);

    //GlobalPool
    GlobalPool::GetInstance()->CVehiclePool.preAllocate(50);
    GlobalPool::GetInstance()->CPedPool.preAllocate(50);
    GlobalPool::GetInstance()->resourcesPool.preAllocate(1500);
    GlobalPool::GetInstance()->ybnPool.preAllocate(1000);
    GlobalPool::GetInstance()->yddPool.preAllocate(1000);
    GlobalPool::GetInstance()->ydrPool.preAllocate(5500);
    GlobalPool::GetInstance()->yftPool.preAllocate(1000);
    GlobalPool::GetInstance()->ytdPool.preAllocate(1500);
    GlobalPool::GetInstance()->ymapPool.preAllocate(1000);
    /*WaterMeshes.reserve(data.WaterQuads.size());
    for (auto& WaterQuad : data.WaterQuads)
    {
        Water water(WaterQuad);
        WaterMeshes.push_back(water);
    }*/

    for (auto& ynd : data.entries[ynd])
    {
    }

    gameHour = 10;
    gameMinute = 0;

    resourceManager->getYtd(3403519606);  //water
    resourceManager->getYtd(4096714883);  //PLAYER YTD

    YddLoader* playerYDD = resourceManager->getYdd(4096714883);
    skydome = resourceManager->getYdd(2640562617);

    while (!skydome->Loaded || !playerYDD->Loaded)
    {
        loadQueuedResources();
    }

    resourceManager->getYtd(GenHash("mapdetail"));
    resourceManager->getYtd(GenHash("vehshare"));
    resourceManager->getYtd(GenHash("vehshare_worn"));
    resourceManager->getYtd(GenHash("vehshare_army"));
    resourceManager->getYtd(GenHash("vehshare_truck"));

    /*for (auto& ytd : data.GtxdEntries)
	{
	 resourceManager->getYtd(ytd.second);
	}*/

    addPedToWorld(glm::vec3(-19.09, -407.75, 48.77), playerYDD);
    //addPedToWorld(glm::vec3(1705.95, 3746.39, 37.64), playerYDD);
    addPedToWorld(glm::vec3(9.66, -1184.98, 75.74), playerYDD);
    addPedToWorld(glm::vec3(2250.18f, 3471.40f, 56.50f), playerYDD);

    //minimap
    /*std::vector<YddLoader*> minimapsYDD;
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_0_2")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_0_3")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_0_4")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_0_5")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_0_6")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_1")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_2")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_3")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_4")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_5")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_6")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_7")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_1_8")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_0")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_1")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_2")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_3")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_4")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_5")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_6")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_7")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_2_8")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_0")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_1")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_2")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_3")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_4")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_5")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_6")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_7")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_3_8")));
    minimapsYDD.push_back(resourceManager->getYdd(GenHash("minimap_4_0")));

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
            building.ydr = ydr.second;
            minimap.push_back(building);
        }
    }*/
}

GameWorld::~GameWorld()
{
}

void GameWorld::updateObjects(Camera* camera, glm::vec3& position)
{
    culledYmaps = 0;
    for (auto& ymap : curYmaps)
    {
        if (ymap->Loaded)
        {
            if (!camera->ContainsAABBNoClipNoOpt(ymap->_CMapData.entitiesExtentsMin, ymap->_CMapData.entitiesExtentsMax))
            {
                ++culledYmaps;
                continue;
            }
            for (auto& object : ymap->objects)
            {
                float Dist = glm::length2(position - object.getPosition());
                bool IsVisible = Dist <= object.entityDef.lodDist * LODMultiplier;
                bool childrenVisible = (Dist <= object.entityDef.childLodDist * LODMultiplier) && (object.entityDef.numChildren > 0);
                if (IsVisible && !childrenVisible)
                {
                    if (!object.loaded)
                    {
                        switch (object.archetype->BaseArchetypeDef.assetType)
                        {
                            case ASSET_TYPE_DRAWABLE:
                            {
                                if (!object.ydr)
                                {
                                    object.ytd = resourceManager->getYtd(object.archetype->BaseArchetypeDef.textureDictionary);
                                    object.ydr = resourceManager->getYdr(object.entityDef.archetypeName);
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

                                    object.loaded = true;
                                }
                                break;
                            }
                            case ASSET_TYPE_DRAWABLEDICTIONARY:
                            {
                                if (!object.ydd)
                                {
                                    object.ytd = resourceManager->getYtd(object.archetype->BaseArchetypeDef.textureDictionary);
                                    object.ydd = resourceManager->getYdd(object.archetype->BaseArchetypeDef.drawableDictionary);
                                }
                                if (object.ydd->Loaded)
                                {
                                    auto iter2 = object.ydd->ydrFiles.find(object.entityDef.archetypeName);
                                    if (iter2 != object.ydd->ydrFiles.end())
                                    {
                                        object.ydr = iter2->second;
                                        object.loaded = true;
                                    }
                                }
                                break;
                            }
                            case ASSET_TYPE_FRAGMENT:
                            {
                                if (!object.yft)
                                {
                                    object.ytd = resourceManager->getYtd(object.archetype->BaseArchetypeDef.textureDictionary);
                                    object.yft = resourceManager->getYft(object.entityDef.archetypeName);
                                }
                                if (object.yft->Loaded)
                                {
                                    object.ydr = object.yft->ydr;
                                    object.loaded = true;
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
                        switch (object.entityDef.flags)
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

                        if (object.archetype->getType() == 1)
                        {  //	TIME ARCHETYPE
                            //	if ((object.Archetype._TimeArchetypeDef.timeFlags >> gameHour) & 1)
                            //{
                            continue;
                            //}
                        }

                        if (camera->intersects(object.boundPos, object.boundRadius))
                        {
                            renderList.push_back(&object);
                        }
                    }
                }
            }

            //	NOTE::
            //
            //	RANDOM VEHICLE spawns in RANDOM PLACE on the same CCarGens IN real game (spawns/despawns based on distance to player, checked on the same place)
            //
            //
            //

            /*int maxSpawn = 10;
            auto MaximumAvailableVehicles = maxSpawn - vehicles.size();

            for (auto& carGen : ymap->CarGenerators)
            {
                if (MaximumAvailableVehicles == 0)
                    break;
                --MaximumAvailableVehicles;

                float radius = 50.f;
                //float radiusNear = radius / 2.0f;

                float dist = glm::distance2(carGen.position, position);
                if (dist < radius * radius)
                {
                    //no other vehicles in the same carGenerator
                    for (auto& veh : vehicles)
                    {
                        if (glm::distance2(carGen.position, veh->getPosition()) < 100 * 100)
                            break;
                    }

                    auto result = glm::atan(carGen.orientY, carGen.orientX);
                    glm::quat test(glm::vec3(0.0f, 0.f, result));
                    // Orientation = Quaternion.RotationYawPitchRoll(0.0f, 0.0f, angl);
                    //float xRandom = RandomFloat(carGen.position.x - carGen.perpendicularLength, carGen.position.x + carGen.perpendicularLength);
                    //float yRandom = RandomFloat(carGen.position.y - carGen.perpendicularLength, carGen.position.y + carGen.perpendicularLength);
                    //if (!camera->intersects(glm::vec3(xRandom, yRandom, carGen.position.z), 1.0f)) {
                    createVehicle(glm::vec3(carGen.position.x, carGen.position.y, carGen.position.z), test);
                    //}
                }
            }*/
        }
    }

    for (auto& ped : peds)
    {
        //if (camera->intersects(ped.position, 1.0f))
        renderList.push_back(ped);
    }

    for (auto& vehicle : vehicles)
    {
        if (camera->intersects(vehicle->getPosition(), 1.0f))
            renderList.push_back(vehicle);
    }

    /*for (int i = 0; i < minimap.size(); i++)
    {
        renderList.push_back(&minimap[i]);
    }*/

    glm::vec3 camPosition = camera->getPosition();
    std::sort(renderList.begin(), renderList.end(), [&camPosition](CEntity* a, CEntity* b) {  // FRONT_TO_BACK
        glm::vec3 lhsPosition = glm::vec3(a->getMatrix()[3]);
        glm::vec3 rhsPosition = glm::vec3(b->getMatrix()[3]);

        return glm::distance2(lhsPosition, camPosition) < glm::distance2(rhsPosition, camPosition);
    });
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
    auto cellID = spaceGrid.getCellPos(PlayerPos);
    auto NodeCell = nodeGrid.getCellPos(PlayerPos);
    auto NavCell = navGrid.getCellPos(PlayerPos);

    if (curNavCell != NavCell)
    {
        curNavCell = NavCell;

        //auto& cell = navGrid.GetCell(NavCell);
    }

    if (curNodeCell != NodeCell)
    {
        curNodeCell = NodeCell;
        curNode = data.nodes[curNodeCell.x * 32 + curNodeCell.y].get();
        //auto& cell = nodeGrid.GetCell(NodeCell);
    }

    if (curCell != cellID)
    {
        //	printf("NEW CELL\n");
        curCell = cellID;

        for (auto& ybn : curYbns)
        {
            ybn->RefCount--;
        }

        for (auto& map : curYmaps)
        {
            map->RefCount--;
        }

        curYbns.clear();
        curYmaps.clear();
        //	Clear previous Ybns

        //SpaceGridCell& cell = spaceGrid.GetCell(cellID);

        //	Bounds
        for (int i = 0; i < data.cacheFile->allBoundsStoreItems.size(); i++)
        {
            glm::i32vec2 min = spaceGrid.getCellPos(data.cacheFile->allBoundsStoreItems[i].Min);
            glm::i32vec2 max = spaceGrid.getCellPos(data.cacheFile->allBoundsStoreItems[i].Max);

            if (cellID.x <= max.x && cellID.x >= min.x && cellID.y <= max.y && cellID.y >= min.y)
            {
                curYbns.emplace_back(resourceManager->getYbn(data.cacheFile->allBoundsStoreItems[i].Name));
            }
        }

        for (int i = 0; i < data.cacheFile->allMapNodes.size(); i++)
        {
            glm::i32vec2 min = spaceGrid.getCellPos(data.cacheFile->allMapNodes[i].streamingExtentsMin);  //	NOT entitiesExtents !!!! GIVES WEIRD RESULTS
            glm::i32vec2 max = spaceGrid.getCellPos(data.cacheFile->allMapNodes[i].streamingExtentsMax);

            if (cellID.x <= max.x && cellID.x >= min.x && cellID.y <= max.y && cellID.y >= min.y)
            {
                curYmaps.emplace_back(resourceManager->getYmap(data.cacheFile->allMapNodes[i].Name));
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

    resourceManager->updateResourceCache();

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
        resourcesThread.pop();

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

        GlobalPool::GetInstance()->resourcesPool.remove(res);

        auto new_time = std::chrono::steady_clock::now();
        diffms = std::chrono::duration_cast<std::chrono::microseconds>(new_time - old_time).count();
    }
}

void GameWorld::createPedestrian()
{
    //	Player *newPlayer = new Player(glm::vec3(0, 20, 0), dynamicsWorld);
    //	peds.push_back(newPlayer);
}

void GameWorld::createVehicle(glm::vec3 position, glm::quat rotation)
{
    int vehicleID = rand() % data.VehiclesInfo.size();

    if (YftLoader* vehicle = resourceManager->getYft(data.VehiclesInfo[vehicleID].Hash); vehicle->Loaded)
    {
        if (vehicle->ydr)
        {
            addVehicleToWorld(position, rotation, data.VehiclesInfo[vehicleID].mass, vehicle);
            printf("Car Spawned\n");
        }
    }
}

void GameWorld::cleanupTraffic(Camera* camera)
{
    float radiusTraffic = 200.0f;
    //	peds
    /*for (auto it = peds.begin() + 3; it != peds.end();)
    {
        if (glm::distance2(camera->getPosition(), (*it)->getPosition()) >= radiusTraffic * radiusTraffic)
        {
            RemovePedFromWorld(*it);
            GlobalPool::GetInstance()->CpedPool.remove(*it);
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
        if (glm::distance2(camera->getPosition(), vehicles[i]->getPosition()) >= radiusTraffic * radiusTraffic)
        {
            removeVehicleFromWorld(vehicles[i]);
            GlobalPool::GetInstance()->CVehiclePool.remove(vehicles[i]);
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

    int maxSpawn = 30;

    auto MaximumAvailableVehicles = maxSpawn - vehicles.size();

    if (curNode)
    {
        //    printf("WE AHVE ONE\n");

        for (auto& node : curNode->nodes)
        {
            if (MaximumAvailableVehicles == 0)
                break;

            glm::vec3 nodePosition(node.PositionX / 4.0f, node.PositionY / 4.0f, node.PositionZ / 32.0f);

            if (glm::distance2(camera->getPosition(), nodePosition) < radiusTraffic * radiusTraffic)
            {
                bool CarIsFoundNear = false;
                for (auto& veh : vehicles)
                {
                    if (glm::distance2(nodePosition, veh->getPosition()) < 5.f * 5.f)
                    {
                        CarIsFoundNear = true;
                        break;
                    }
                }
                if (!CarIsFoundNear)
                {
                    --MaximumAvailableVehicles;

                    createVehicle(nodePosition, glm::quat(0.0, 0.0, 0.0, -1.0));
                }
            }
        }
    }
}

void GameWorld::updateDynamicObjects()
{
    /*for (auto & map : curYmaps)
	{
	 if (map->Loaded) {
	  for (auto & object : map->Objects)
	  {
	   if (object.loaded) {
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
/*if (nodeGrid.cells[curNodeCell.x * 32 + curNodeCell.y]->ynd)
	{

	 for (auto& node : nodeGrid.cells[curNodeCell.x * 32 + curNodeCell.y]->ynd->nodes)
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
	  }
	 }
	}*/
//}

CVehicle* GameWorld::findNearestVehicle()
{
    float d = 30.0f;

    CVehicle* nearestVehicle = nullptr;

    for (auto& vehicle : vehicles)
    {
        float vd = glm::length2(peds[currentPlayerID]->getPosition() - vehicle->getPosition());
        if (vd < d * d)
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
    cleanupTraffic(camera);
    createTraffic(camera);

    for (auto& ped : peds)
    {
        //physicsSystem.getPhysicsWorld()->updateSingleAabb(ped->getPhysCharacter());
        ped->physicsTick();
    }

    for (auto& vehicle : vehicles)
    {
        //physicsSystem.getPhysicsWorld()->updateSingleAabb(vehicle->getCarChassisRigidbody());
        vehicle->physicsTick();
    }

    if (enableStreaming)
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
    const int WATER_WORLD_SIZE = 10000.f;
    const int WATER_HQ_DATA_SIZE = 128;
    auto wX = static_cast<int>((Position.x + WATER_WORLD_SIZE / 2.f) /
                               (WATER_WORLD_SIZE / WATER_HQ_DATA_SIZE));
    auto wY = static_cast<int>((Position.y + WATER_WORLD_SIZE / 2.f) /
                               (WATER_WORLD_SIZE / WATER_HQ_DATA_SIZE));

    int i = (wX * WATER_HQ_DATA_SIZE) + wY;
    if (data.waterPosition[i])
    {
        printf("IN WATER\n");
    }
    else
    {
    }
    return false;
}

void GameWorld::addVehicleToWorld(glm::vec3 position, glm::quat rot, float mass, YftLoader* model)
{
    CVehicle* vehicle = GlobalPool::GetInstance()->CVehiclePool.create(position, rot, mass, model, physicsSystem.getPhysicsWorld());
    physicsSystem.addVehicle(vehicle);

    vehicles.push_back(vehicle);
}

void GameWorld::removeVehicleFromWorld(CVehicle* vehicle)
{
    physicsSystem.removeVehicle(vehicle);
}

void GameWorld::addPedToWorld(glm::vec3 pos, YddLoader* model)
{
    CPed* ped = GlobalPool::GetInstance()->CPedPool.create(pos, model);
    physicsSystem.addPed(ped);
    peds.push_back(ped);
}

void GameWorld::removePedFromWorld(CPed* ped)
{
    physicsSystem.removePed(ped);
}

void GameWorld::testFunction(glm::vec3 position)
{
    /*for (auto& ytd : data.GtxdEntries)
	{
	 while (!resourceManager->getYtd(ytd.second))
	 {
	  LoadQueuedResources();
	 }
	}
	printf("DONE\n");*/
}
