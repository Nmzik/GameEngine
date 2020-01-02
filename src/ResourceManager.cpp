#include "ResourceManager.h"

#include "CBuilding.h"
#include "Game.h"
#include "GameData.h"
#include "GameWorld.h"

#include "loaders/RpfEntry.h"
#include "loaders/YbnLoader.h"
#include "loaders/YddLoader.h"
#include "loaders/YdrLoader.h"
#include "loaders/YftLoader.h"
#include "loaders/YmapLoader.h"
#include "loaders/YndLoader.h"
#include "loaders/YscLoader.h"
#include "loaders/YtdLoader.h"
#include "loaders/YtypLoader.h"

// OVERALL
// 50MB for BULLET PHYSICS
// 50MB for ResourceLoader

static void* threadEntryFunc(void* This)
{
    ((ResourceManager*)This)->update();
    return NULL;
}

ResourceManager::ResourceManager(GameData* gameData, Game* mainGame)
    : running(true)
    , data{*gameData}
    , game(*mainGame)
{
    // printf("RESOURCE MANAGER loaded!\n");

    // yndLoader.reserve(500);
    // ytypLoader.reserve(500);
    ydrLoader.reserve(500);
    yddLoader.reserve(500);
    yftLoader.reserve(500);
    ytdLoader.reserve(1500);
    ybnLoader.reserve(50);
    ymapLoader.reserve(500);

    resource_allocator = std::make_unique<ThreadSafeAllocator>(50 * 1024 * 1024);

    loadThread = Thread(&threadEntryFunc, this);
}

ResourceManager::~ResourceManager()
{
    Resource* exitResource = GlobalPool::GetInstance()->resourcesPool.create(Type::null, 0, nullptr);
    addToWaitingList(exitResource);

    loadThread.join();
}

void ResourceManager::getGtxd(uint32_t hash)
{
    /* auto iter = data.GtxdEntries.find(hash);
	 if (iter != data.GtxdEntries.end()) {
	  auto it = ytdLoader.find(iter->second);
	  if (it == ytdLoader.end()) {
	   ytdLoader[iter->second] = new YtdLoader();
	   AddToWaitingList(Resource(ytd, iter->second, 0, nullptr));
	  }
	 }*/
}

YtdLoader* ResourceManager::getYtdAsync(uint32_t hash)
{
    YtdLoader* resource = getYtd(hash);

    while (!resource->isLoaded())
    {
        loadQueuedResources();
    }

    return resource;
}

YddLoader* ResourceManager::getYddAsync(uint32_t hash)
{
    YddLoader* resource = getYdd(hash);

    while (!resource->isLoaded())
    {
        loadQueuedResources();
    }

    return resource;
}

YmapLoader* ResourceManager::getYmap(uint32_t hash)
{
    auto it = ymapLoader.find(hash);
    if (it != ymapLoader.end())
    {
        it->second->refCount++;
        return it->second;
    }
    else
    {
        YmapLoader* loader = GlobalPool::GetInstance()->ymapPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ymap, hash, loader));
        loader->refCount++;
        ymapLoader.insert({hash, loader});

        return loader;
    }
}

YdrLoader* ResourceManager::getYdr(uint32_t hash)
{
    auto iter = ydrLoader.find(hash);
    if (iter != ydrLoader.end())
    {
        iter->second->refCount++;
        return iter->second;
    }
    else
    {
        YdrLoader* loader = GlobalPool::GetInstance()->ydrPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ydr, hash, loader));
        loader->refCount++;
        ydrLoader.insert({hash, loader});

        return loader;
    }
}

YtdLoader* ResourceManager::getYtd(uint32_t hash)
{
    auto iter = ytdLoader.find(hash);
    if (iter != ytdLoader.end())
    {
        iter->second->refCount++;
        return iter->second;
    }
    else
    {
        YtdLoader* dependency = nullptr;
        auto iter = data.gtxdEntries.find(hash);
        if (iter != data.gtxdEntries.end())
        {
            //fix memory leak!
            dependency = getYtd(iter->second);
        }
        
        YtdLoader* loader = GlobalPool::GetInstance()->ytdPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ytd, hash, loader));
        loader->refCount++;
        ytdLoader.insert({hash, loader});

        return loader;
    }
}

YddLoader* ResourceManager::getYdd(uint32_t hash)
{
    auto iter = yddLoader.find(hash);
    if (iter != yddLoader.end())
    {
        iter->second->refCount++;
        return iter->second;
    }
    else
    {
        YddLoader* loader = GlobalPool::GetInstance()->yddPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ydd, hash, loader));
        loader->refCount++;
        yddLoader.insert({hash, loader});

        return loader;
    }
}

YftLoader* ResourceManager::getYft(uint32_t hash)
{
    auto iter = yftLoader.find(hash);
    if (iter != yftLoader.end())
    {
        iter->second->refCount++;
        return iter->second;
    }
    else
    {
        YftLoader* loader = GlobalPool::GetInstance()->yftPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(yft, hash, loader));
        loader->refCount++;
        yftLoader.insert({hash, loader});

        return loader;
    }
}

YbnLoader* ResourceManager::getYbn(uint32_t hash)
{
    auto iter = ybnLoader.find(hash);
    if (iter != ybnLoader.end())
    {
        iter->second->refCount++;
        return iter->second;
    }
    else
    {
        YbnLoader* loader = GlobalPool::GetInstance()->ybnPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ybn, hash, loader));
        loader->refCount++;
        ybnLoader.insert({hash, loader});
        return loader;
    }
}

YscLoader* ResourceManager::getYsc(uint32_t hash)
{
    auto iter = yscLoader.find(hash);
    if (iter != yscLoader.end())
    {
        iter->second->refCount++;
        return iter->second;
    }
    else
    {
        YscLoader* loader = new YscLoader();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ysc, hash, loader));
        loader->refCount++;
        yscLoader.insert({hash, loader});
        return loader;
    }
}

inline void ResourceManager::addToMainQueue(Resource* res)
{
    Lock_guard lock(&mainThreadLock);
    mainThreadResources.push(res);
}

void ResourceManager::addToWaitingList(Resource* res)
{
    Lock_guard lock(&loadThreadLock);
    waitingList.push(res);
    loadCondition.notify();
}

inline Resource* ResourceManager::removeFromWaitingList()
{
    Lock_guard lock(&loadThreadLock);

    while (waitingList.empty())
        loadCondition.wait(&loadThreadLock);

    Resource* res = waitingList.pop();
    return res;
}

void ResourceManager::update()
{
    while (running)
    {
        Resource* res = removeFromWaitingList();

        switch (res->type)
        {
            case ydr:
            case ydd:
            case yft:
            case ytd:
            case ybn:
            case ymap:
            case ynd:
            case ynv:
            case ycd:
            case ysc:
            {
                auto it = data.entries[res->type].find(res->hash);
                if (it != data.entries[res->type].end())
                {
                    uint8_t* allocatedMemory = nullptr;

                    while (!allocatedMemory)
                    {
                        allocatedMemory = (uint8_t*)resource_allocator->allocate(it->second->UncompressedFileSize, 16);
                        //if (!allocatedMemory)
                        //printf("TRYING AGAIN\n");
                    }

                    //printf("DONE\n");

                    res->buffer = allocatedMemory;
                    res->bufferSize = it->second->UncompressedFileSize;
                    data.extractFileResource(*(it->second), res->buffer, res->bufferSize);
                    res->systemSize = (it->second->SystemSize);

                    switch (res->type)
                    {
                        case ymap:
                        {
                            YmapLoader* iter = static_cast<YmapLoader*>(res->file);
                            memstream stream(&res->buffer[0], res->bufferSize);
                            iter->init(stream);

                            for (auto& mlo : iter->CMloInstanceDefs)
                            {
                                auto it = data.mloDictionary.find(mlo.fwEntityDef.archetypeName);
                                if (it != data.mloDictionary.end())
                                {
                                    for (auto& entityDef : it->second)
                                    {
                                        /*glm::quat rotmultiply = entityDef.rotation * mlo.fwEntityDef.rotation;
                                        rotmultiply.w = -rotmultiply.w;
                                        glm::mat4 matrix = glm::translate(glm::mat4(1.0f), mlo.fwEntityDef.position + entityDef.position) *
                                                           glm::mat4_cast(glm::quat(-mlo.fwEntityDef.rotation.w, -mlo.fwEntityDef.rotation.x,
                                                                                    -mlo.fwEntityDef.rotation.y, -mlo.fwEntityDef.rotation.z)) *
                                                           glm::scale(glm::mat4(1.0f),
                                                                      glm::vec3(entityDef.scaleXY, entityDef.scaleXY, entityDef.scaleZ));
                                        entityDef.position = mlo.fwEntityDef.position + entityDef.position;
                                        entityDef.rotation = rotmultiply;
                                        iter->entities.emplace_back(entityDef);*/
                                    }
                                }
                            }

                            for (auto& object : iter->entities)
                            {
                                std::unordered_map<uint32_t, fwArchetype*>::iterator it = data.archetypes.find(object.entityDef.archetypeName);
                                if (it != data.archetypes.end())
                                {
                                    if (it->second->getType() == 2)
                                    {
                                        //printf("");
                                    }

                                    object.archetype = it->second;

                                    object.boundPos = object.entityDef.position + object.archetype->BaseArchetypeDef.bsCentre;
                                    object.boundRadius = object.archetype->BaseArchetypeDef
                                                             .bsRadius;  //* std::max(object.CEntity.scaleXY, object.CEntity.scaleZ); TREES doesnt render with multiplying by scale

                                    if (object.entityDef.lodDist <= 0)
                                        object.entityDef.lodDist = it->second->BaseArchetypeDef.lodDist;
                                    if (object.entityDef.childLodDist <= 0)
                                        object.entityDef.childLodDist = it->second->BaseArchetypeDef.lodDist;
                                }
                                else
                                {
                                    //    printf("ERROR\n"); ACTUALLY IT CAN HAPPEN
                                    object.archetype = nullptr;
                                    object.entityDef.lodDist = 0.f;  //    HACK = DONT RENDER objects WITH UNKNOWN ARCHETYPE
                                }

                                object.entityDef.lodDist *= object.entityDef.lodDist;            // glm::length2
                                object.entityDef.childLodDist *= object.entityDef.childLodDist;  // glm::length2
                            }
                            break;
                        }
                        case ydr:
                        case ydd:
                        case yft:
                        {
                            memstream stream(&res->buffer[0], res->bufferSize);
                            res->file->init(stream);
                            break;
                        }
                        case ytd:
                        {
                            memstream stream(&res->buffer[0], res->bufferSize);
                            TextureDictionary* dictionary = (TextureDictionary*)&res->buffer[0];
                            dictionary->Resolve(stream);
                            break;
                        }
                        default:
                            break;
                    }
                }
                addToMainQueue(res);
            }
            break;
            case awc:
                break;
            case null:
            {
                printf("EXIT HAS BEEN CALLED\n");
                running = false;
            }
            break;
        }
    }
}

void ResourceManager::removeAll()
{
    for (auto it = ytdLoader.begin(); it != ytdLoader.end();)
    {
        it = ytdLoader.erase(it);
    }
}

void ResourceManager::updateResourceCache(GameWorld* world)
{
    //	printf("FREE SPACE %zd\n", _main_allocator->getSize() - _main_allocator->getUsedMemory());
    // REMOVE objects WHEN WE ARE IN ANOTHER CELL????  RUN GARBAGE COLLECTOR WHEN IN ANOTHER CEL
    for (auto it = ybnLoader.begin(); it != ybnLoader.end();)
    {
        if ((it->second)->refCount == 0 && (it->second)->isLoaded())
        {
            if ((it->second)->getRigidBody())
                world->getPhysicsSystem()->removeRigidBody((it->second)->getRigidBody());
            GlobalPool::GetInstance()->ybnPool.remove(it->second);
            it = ybnLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = ymapLoader.begin(); it != ymapLoader.end();)
    {
        if ((it->second)->refCount == 0 && (it->second)->isLoaded())
        {
            for (auto& entity : (it->second)->entities)
            {
                world->unloadEntity(entity);
            }

            GlobalPool::GetInstance()->ymapPool.remove(it->second);
            it = ymapLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }

    //printf("YDR SIZE %lu\n", ydrLoader.size());
    for (auto it = ydrLoader.begin(); it != ydrLoader.end();)
    {
        if ((it->second)->refCount == 0 && (it->second)->isLoaded())
        {
            GlobalPool::GetInstance()->ydrPool.remove(it->second);
            it = ydrLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }

    //printf("YDD SIZE %lu\n", yddLoader.size());
    for (auto it = yddLoader.begin(); it != yddLoader.end();)
    {
        if ((it->second)->refCount == 0 && (it->second)->isLoaded())
        {
            GlobalPool::GetInstance()->yddPool.remove(it->second);
            it = yddLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = yftLoader.begin(); it != yftLoader.end();)
    {
        if ((it->second)->refCount == 0 && (it->second)->isLoaded())
        {
            GlobalPool::GetInstance()->yftPool.remove(it->second);
            it = yftLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }
    //printf("YTD SIZE %lu\n", ytdLoader.size());
    for (auto it = ytdLoader.begin(); it != ytdLoader.end();)
    {
        if ((it->second)->refCount == 0 && (it->second)->isLoaded())
        {
            /*YtdLoader* dependency = it->second->dependency;
            
            while (dependency) {
                if (dependency->refCount > 0)
                    dependency->refCount = dependency->refCount - 2;
                if (dependency->refCount == 0)
                    dependency = dependency->dependency;
                else
                    dependency = nullptr;
            }*/

            GlobalPool::GetInstance()->ytdPool.remove(it->second);
            it = ytdLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ResourceManager::loadQueuedResources()
{
    //    If we still didn't finish loading our queue, do not swap! Swap only if we dont have any job.
    if (tempMainThreadResources.empty())
    {
        Lock_guard swapLock(&mainThreadLock);
        if (!mainThreadResources.empty())
            mainThreadResources.swap(tempMainThreadResources);
    }

    if (!tempMainThreadResources.empty())
    {
        //numFiles++;
        Resource* res = tempMainThreadResources.pop();

        if (res->bufferSize != 0)
        {
            memstream stream(&res->buffer[0], res->bufferSize);
            stream.systemSize = res->systemSize;
            switch (res->type)
            {
                case ymap:
                {
                    res->file->setLoaded();
                    break;
                }
                case ydr:
                case ydd:
                case yft:
                {
                    res->file->finalize(game.getRenderer(), stream);
                    break;
                }
                case ytd:
                {
                    YtdLoader* loader = static_cast<YtdLoader*>(res->file);
                    TextureDictionary* texDictionary = (TextureDictionary*)&res->buffer[0];
                    
                    loader->finalize(game.getRenderer(), texDictionary, res->systemSize);
                    
                    break;
                }
                case ybn:
                {
                    YbnLoader* ybn = static_cast<YbnLoader*>(res->file);
                    
                    phBound* boundData = (phBound*)&res->buffer[0];
                    ybn->finalize(game.getRenderer(), boundData, stream);
                    game.getWorld()->getPhysicsSystem()->addRigidBody(ybn->getRigidBody());  //    NOT THREAD SAFE!
                    break;
                }
                case ysc:
                {
                    res->file->init(stream);
                    break;
                }
                case ynd:
                case ynv:
                case ycd:
                case awc:
                case null:
                    break;
            }
            res->file->setLoaded();

            resource_allocator->deallocate(res->buffer);
        }

        GlobalPool::GetInstance()->resourcesPool.remove(res);

        //auto new_time = std::chrono::steady_clock::now();
        //diffms = std::chrono::duration_cast<std::chrono::microseconds>(new_time - old_time).count();
    }

    //if (numFiles > 0)
    // printf("%d Files Loaded\n", numFiles);
}
