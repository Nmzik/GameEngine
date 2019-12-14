#include "GameWorld.h"

#include "CBuilding.h"
#include "Camera.h"
#include "GTAEncryption.h"
#include "GameData.h"
#include "ResourceManager.h"
#include "common.h"

#include "loaders/YbnLoader.h"
#include "loaders/YddLoader.h"
#include "loaders/YftLoader.h"
#include "loaders/YmapLoader.h"
#include "loaders/YndLoader.h"
#include "loaders/YnvLoader.h"
#include "loaders/YtdLoader.h"
#include "loaders/YtypLoader.h"

GameWorld::GameWorld(ResourceManager* resManager)
    : resourceManager(resManager)
    , curNode(nullptr)
    , dirLight(glm::vec3(0.1f, 0.8f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), true)
{
    /*for (auto& vehicle : data.VehiclesInfo)
     {
     vehiclesPool[vehicle.Hash] = vehicle;
     }*/
    {
        CacheDatFile* cache = resourceManager->getGameData()->cacheFile.get();
        mapDataStore.Init(cache->allMapNodes);
        boundsStore.Init(cache->allBoundsStoreItems);
    }

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
    GlobalPool::GetInstance()->ytdPool.preAllocate(2000);
    GlobalPool::GetInstance()->ymapPool.preAllocate(1000);
    /*WaterMeshes.reserve(data.WaterQuads.size());
     for (auto& WaterQuad : data.WaterQuads)
     {
     Water water(WaterQuad);
     WaterMeshes.push_back(water);
     }*/

    /*for (auto& ynd : data.entries[ynd])
     {
     }*/

    gameHour = 10;
    gameMinute = 0;
}

GameWorld::~GameWorld()
{
}

void GameWorld::postLoad()
{
    resourceManager->getYtdAsync(3403519606);  //water
    resourceManager->getYtdAsync(4096714883);  //PLAYER YTD

    YddLoader* playerYDD = resourceManager->getYddAsync(4096714883);
    //
    skydome = resourceManager->getYddAsync(2640562617);
    skydomeYTD = resourceManager->getYtdAsync(2640562617);

    resourceManager->getYtdAsync(GenHash("mapdetail"));
    resourceManager->getYtdAsync(GenHash("vehshare"));
    resourceManager->getYtdAsync(GenHash("vehshare_worn"));
    resourceManager->getYtdAsync(GenHash("vehshare_army"));
    resourceManager->getYtdAsync(GenHash("vehshare_truck"));

    /*for (auto& ytd : resourceManager->getGameData()->gtxdEntries)
     {
     resourceManager->getYtd(ytd.second);
     }*/
    CPed* ped1 = new CPed(glm::vec3(9.66, -1184.98, 75.74), playerYDD);
    ped1->initDrawable();
    CPed* ped2 = new CPed(glm::vec3(9.66, -1184.98, 75.74), playerYDD);
    ped2->initDrawable();
    CPed* ped3 = new CPed(glm::vec3(2250.18f, 3471.40f, 56.50f), playerYDD);
    ped3->initDrawable();

    addPedToWorld(ped1);
    addPedToWorld(ped2);
    addPedToWorld(ped3);
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
     while (!minimapsYDD[i]->loaded)
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

void GameWorld::updateObjects(Camera* camera, glm::vec3& position)
{
    culledYmaps = 0;
    for (auto& ymap : curYmaps)
    {
        if (ymap->isLoaded())
        {
            if (!camera->ContainsAABBNoClipNoOpt(ymap->cMapData.entitiesExtentsMin, ymap->cMapData.entitiesExtentsMax))
            {
                ++culledYmaps;
                continue;
            }
            for (auto& object : ymap->entities)
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
                                if (object.ydr->isLoaded())
                                {
                                    //	NOTE:
                                    //
                                    //	SPAWN OBJECTS STATICALLY (IN SLEEP STATE)
                                    //
                                    //	SUPER DIRTY NEED FIX URGENT! UGLY FIX!!!

                                    if (object.ydr->ybn)
                                    {
                                        btCompoundShape* compound = object.ydr->ybn->compound;

                                        if (compound->getNumChildShapes() != 0)
                                        {
                                            //SET POSITION OF COLLISION TO OBJECT PLACE
                                            //NOTE: SPAWN AT EXACT SAME PLACE!!! "NO +1.0f"
                                            btVector3 localInertia(0, 0, 0);
                                            float mass = 0.0f;

                                            btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(object.rotation.w, object.rotation.x,
                                                                                                                                  object.rotation.y, object.rotation.z),
                                                                                                                     btVector3(object.position.x, object.position.y, object.position.z)));
                                            btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, MotionState, compound, localInertia);
                                            object.rigidBody =
                                                new btRigidBody(groundRigidBodyCI);
                                            getPhysicsSystem()->addRigidBody(object.rigidBody);
                                        }
                                    }

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
                                if (object.ydd->isLoaded())
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
                                if (object.yft->isLoaded())
                                {
                                    object.ydr = object.yft->ydr;
                                    object.loaded = true;
                                }
                                break;
                            }
                            case ASSET_TYPE_UNINITIALIZED:
                            case ASSET_TYPE_ASSETLESS:
                                break;
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
                else
                {
                    unloadEntity(object);
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
        if (camera->intersects(ped->getPosition(), 1.0f))
            renderList.push_back(ped);
    }

    for (auto& vehicle : vehicles)
    {
        if (camera->intersects(vehicle->getPosition(), 1.0f))
            renderList.push_back(vehicle);
    }

    glm::vec3 camPosition = camera->getPosition();
    std::sort(renderList.begin(), renderList.end(), [&camPosition](CEntity* a, CEntity* b) {  // FRONT_TO_BACK
        glm::vec3 lhsPosition = glm::vec3(a->getMatrix()[3]);
        glm::vec3 rhsPosition = glm::vec3(b->getMatrix()[3]);

        return glm::distance2(lhsPosition, camPosition) < glm::distance2(rhsPosition, camPosition);
    });
}

void GameWorld::unloadEntity(CBuilding& object)
{
    if (object.rigidBody)
    {
        getPhysicsSystem()->removeRigidBody(object.rigidBody);
        delete object.rigidBody->getMotionState();
        delete object.rigidBody;

        object.rigidBody = nullptr;
    }
    if (object.archetype)
    {
        switch (object.archetype->BaseArchetypeDef.assetType)
        {
            case ASSET_TYPE_DRAWABLE:
                if (object.ydr)
                {
                    object.ydr->refCount--;
                    object.ydr = nullptr;
                }
                break;
            case ASSET_TYPE_DRAWABLEDICTIONARY:
                if (object.ydd)
                {
                    object.ydd->refCount--;
                    object.ydd = nullptr;
                }
                break;
            case ASSET_TYPE_FRAGMENT:
                if (object.yft)
                {
                    object.yft->refCount--;
                    object.yft = nullptr;
                }
                break;
            case ASSET_TYPE_UNINITIALIZED:
            case ASSET_TYPE_ASSETLESS:
                break;
        }
    }
    if (object.ytd)
    {
        object.ytd->refCount--;
        object.ytd = nullptr;
    }

    object.loaded = false;
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

void GameWorld::getVisibleYmaps(glm::vec3& playerPos)
{
    auto NodeCell = nodeGrid.getCellPos(playerPos);
    auto NavCell = navGrid.getCellPos(playerPos);

    if (curNavCell != NavCell)
    {
        curNavCell = NavCell;

        //auto& cell = navGrid.GetCell(NavCell);
    }

    if (curNodeCell != NodeCell)
    {
        curNodeCell = NodeCell;
        curNode = resourceManager->getGameData()->nodes[curNodeCell.x * 32 + curNodeCell.y].get();
        //auto& cell = nodeGrid.GetCell(NodeCell);
    }

    //<= 25.0f
    const float streamingDistanceReload = 25.f;

    if (glm::distance2(streamingPos, glm::vec2(playerPos)) > streamingDistanceReload * streamingDistanceReload)
    {
        printf("NEW NODE\n");
        streamingPos = glm::vec2(playerPos);
        {
            static std::vector<MapDataStoreNode> nodes;
            nodes.clear();

            mapDataStore.GetItems(nodes, playerPos);

            for (auto& ymap : curYmaps)
            {
                ymap->refCount--;
            }
            curYmaps.clear();

            for (int i = 0; i < nodes.size(); i++)
            {
                curYmaps.push_back(resourceManager->getYmap(nodes[i].Name));
            }
        }
        //
        {
            static std::vector<BoundsStoreItem> nodes;
            nodes.clear();

            boundsStore.GetItems(nodes, playerPos);

            for (auto& ybn : curYbns)
            {
                ybn->refCount--;
            }
            curYbns.clear();

            for (int i = 0; i < nodes.size(); i++)
            {
                curYbns.push_back(resourceManager->getYbn(nodes[i].Name));
            }
        }
    }
    /*if (curCell != cellID)
    {
        //	printf("NEW CELL\n");
        curCell = cellID;

        for (auto& ybn : curYbns)
        {
            ybn->refCount--;
        }

        curYbns.clear();
        //curYmaps.clear();
        //	Clear previous Ybns

        //SpaceGridCell& cell = spaceGrid.GetCell(cellID);
        GameData& data = *resourceManager->getGameData();*/
    //	Bounds
    /*for (int i = 0; i < data.cacheFile->allBoundsStoreItems.size(); i++)
        {
            auto max = data.cacheFile->boundsStoreBoundaries[i].max;
            auto min = data.cacheFile->boundsStoreBoundaries[i].min;

            if (curCell.x <= max.x && curCell.x >= min.x && curCell.y <= max.y && curCell.y >= min.y)
            {
                curYbns.emplace_back(resourceManager->getYbn(data.cacheFile->allBoundsStoreItems[i].Name));
            }
        }*/

    /*for (int i = 0; i < data.cacheFile->allMapNodes.size(); i++)
        {
            auto max = data.cacheFile->mapNodesBoundaries[i].max;
            auto min = data.cacheFile->mapNodesBoundaries[i].min;
            //    NOT entitiesExtents !!!! GIVES WEIRD RESULTS
            if (curCell.x <= max.x && curCell.x >= min.x && curCell.y <= max.y && curCell.y >= min.y)
            {
                curYmaps.emplace_back(resourceManager->getYmap(data.cacheFile->allMapNodes[i].Name));
            }
        }*/

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
}

void GameWorld::createPedestrian()
{
    //	Player *newPlayer = new Player(glm::vec3(0, 20, 0), dynamicsWorld);
    //	peds.push_back(newPlayer);
}

void GameWorld::createVehicle(glm::vec3 position, glm::quat rotation)
{
    GameData& data = *resourceManager->getGameData();

    uint32_t carHash = data.vehiclesInfo.begin()->Hash;

    YftLoader* carModel = resourceManager->getYft(carHash);
    if (carModel->isLoaded())
        addVehicleToWorld(position, rotation, data.vehiclesInfo.begin()->mass, carModel);

    /*int vehicleID = rand() % data.vehiclesInfo.size();

    if (YftLoader* vehicle = resourceManager->getYft(data.vehiclesInfo[vehicleID].Hash); vehicle->isLoaded())
    {
        if (vehicle->ydr)
        {
            addVehicleToWorld(position, rotation, data.vehiclesInfo[vehicleID].mass, vehicle);
            printf("Car Spawned\n");
        }
    }*/
}

void GameWorld::cleanupTraffic(Camera* camera)
{
    float radiusTraffic = 200.0f;
    for (int i = 0; i < vehicles.size(); i++)
    {
        if (glm::distance2(camera->getPosition(), vehicles[i]->getPosition()) >= radiusTraffic * radiusTraffic)
        {
            printf("Car %d should be deleted\n", i);
        }
    }
}

void GameWorld::createTraffic(Camera* camera)
{
    float radiusTraffic = 500.0f;

    int maxSpawn = 30;

    auto MaximumAvailableVehicles = maxSpawn - vehicles.size();

    if (curNode)
    {
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
     if (map->loaded) {
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

    //updateDynamicObjects();
    //cleanupTraffic(camera);
    //createTraffic(camera);

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

        /*if (!isAllCollisionLoaded())
        {
            if (getCurrentPlayer()->getPhysCharacter()->getGravity() != btVector3(0.f, 0.f, 0.f))
                getCurrentPlayer()->getPhysCharacter()->setGravity(btVector3(0.f, 0.f, 0.f));
        }
        else
        {
            if (getCurrentPlayer()->getPhysCharacter()->getGravity() == btVector3(0.f, 0.f, 0.f)) getCurrentPlayer()->getPhysCharacter()->setGravity(physicsSystem.getPhysicsWorld()->getGravity());
        }*/

        //glm::vec3 playerPos = getCurrentPlayer()->getPosition();
        glm::vec3 playerPos = camera->getPosition();

        getVisibleYmaps(playerPos);
        updateObjects(camera, playerPos);
    }
}

bool GameWorld::isAllCollisionLoaded()
{
    for (auto& ybn : curYbns)
    {
        if (!ybn->isLoaded())
            return false;
    }
    return true;
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
    if (resourceManager->getGameData()->waterPosition[i])
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
    //CVehicle* vehicle = GlobalPool::GetInstance()->CVehiclePool.create(position, rot, mass, model, physicsSystem.getPhysicsWorld());
    CVehicle* vehicle = new CVehicle(position, rot, mass, model, physicsSystem.getPhysicsWorld());
    physicsSystem.addVehicle(vehicle);

    vehicles.push_back(vehicle);
}

void GameWorld::removeVehicleFromWorld(CVehicle* vehicle)
{
    physicsSystem.removeVehicle(vehicle);
    //delete vehicle;
}

void GameWorld::addPedToWorld(CPed* ped)
{
    physicsSystem.addPed(ped);
    peds.push_back(ped);
}

void GameWorld::removePedFromWorld(CPed* ped)
{
    physicsSystem.removePed(ped);
}
