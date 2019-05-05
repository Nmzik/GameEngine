#include "ResourceManager.h"
#include "CBuilding.h"
#include "GameData.h"
#include "GameWorld.h"
#include "RpfEntry.h"
#include "YbnLoader.h"
#include "YddLoader.h"
#include "YdrLoader.h"
#include "YftLoader.h"
#include "YmapLoader.h"
#include "YndLoader.h"
#include "YscLoader.h"
#include "YtdLoader.h"
#include "YtypLoader.h"

// OVERALL
// 50MB for BULLET PHYSICS
// 50MB for ResourceLoader

ResourceManager::ResourceManager(GameWorld* world)
    : gameworld{world}
    , running(true)
{
    // printf("RESOURCE MANAGER LOADED!\n");

    // yndLoader.reserve(500);
    // ytypLoader.reserve(500);
    ydrLoader.reserve(500);
    yddLoader.reserve(500);
    yftLoader.reserve(500);
    ytdLoader.reserve(1500);
    ybnLoader.reserve(50);
    ymapLoader.reserve(500);

    resource_allocator = std::make_unique<ThreadSafeAllocator>(50 * 1024 * 1024);

    ResourcesThread = std::thread(&ResourceManager::update, this);
}

ResourceManager::~ResourceManager()
{
    Resource* exitResource = GlobalPool::GetInstance()->resourcesPool.create(Type::null, 0, nullptr);
    addToWaitingList(exitResource);

    ResourcesThread.join();
}

void ResourceManager::GetGtxd(uint32_t hash)
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

YmapLoader* ResourceManager::GetYmap(uint32_t hash)
{
    auto it = ymapLoader.find(hash);
    if (it != ymapLoader.end())
    {
        it->second->RefCount++;
        return it->second;
    }
    else
    {
        YmapLoader* loader = GlobalPool::GetInstance()->ymapPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ymap, hash, loader));
        loader->RefCount++;
        ymapLoader.insert({hash, loader});

        return loader;
    }
}

YdrLoader* ResourceManager::GetYdr(uint32_t hash)
{
    auto iter = ydrLoader.find(hash);
    if (iter != ydrLoader.end())
    {
        iter->second->RefCount++;
        return iter->second;
    }
    else
    {
        YdrLoader* loader = GlobalPool::GetInstance()->ydrPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ydr, hash, loader));
        loader->RefCount++;
        ydrLoader.insert({hash, loader});

        return loader;
    }
}

YtdLoader* ResourceManager::GetYtd(uint32_t hash)
{
    auto it = ytdLoader.find(hash);
    if (it != ytdLoader.end())
    {
        it->second->RefCount++;
        return it->second;
    }
    else
    {
        /*auto iter = gameworld->getGameData()->GtxdEntries.find(hash);
		if (iter != gameworld->getGameData()->GtxdEntries.end())
		{
		 GetYtd(iter->second);
		}*/

        bool HDTextures = false;
        if (HDTextures)
        {
            auto iter = gameworld->getGameData()->HDTextures.find(hash);
            if (iter != gameworld->getGameData()->HDTextures.end())
            {
                auto it = ytdLoader.find(iter->second);
                if (it != ytdLoader.end())
                {
                    it->second->RefCount++;
                    return it->second;
                }
                else
                {
                    YtdLoader* loader = GlobalPool::GetInstance()->ytdPool.create();
                    addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ytd, iter->second, loader));
                    loader->RefCount++;
                    ytdLoader.insert({iter->second, loader});

                    return loader;
                }
            }
            else
            {
                printf("HD Texture not found\n");
            }
        }

        YtdLoader* loader = GlobalPool::GetInstance()->ytdPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ytd, hash, loader));
        loader->RefCount++;
        ytdLoader.insert({hash, loader});

        return loader;
    }
}

YddLoader* ResourceManager::GetYdd(uint32_t hash)
{
    auto iter = yddLoader.find(hash);
    if (iter != yddLoader.end())
    {
        iter->second->RefCount++;
        return iter->second;
    }
    else
    {
        YddLoader* loader = GlobalPool::GetInstance()->yddPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ydd, hash, loader));
        loader->RefCount++;
        yddLoader.insert({hash, loader});

        return loader;
    }
}

YftLoader* ResourceManager::GetYft(uint32_t hash)
{
    auto iter = yftLoader.find(hash);
    if (iter != yftLoader.end())
    {
        iter->second->RefCount++;
        return iter->second;
    }
    else
    {
        YftLoader* loader = GlobalPool::GetInstance()->yftPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(yft, hash, loader));
        loader->RefCount++;
        yftLoader.insert({hash, loader});

        return loader;
    }
}

YbnLoader* ResourceManager::GetYbn(uint32_t hash)
{
    auto iter = ybnLoader.find(hash);
    if (iter != ybnLoader.end())
    {
        iter->second->RefCount++;
        return iter->second;
    }
    else
    {
        YbnLoader* loader = GlobalPool::GetInstance()->ybnPool.create();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ybn, hash, loader));
        loader->RefCount++;
        ybnLoader.insert({hash, loader});
        return loader;
    }
}

YscLoader* ResourceManager::GetYsc(uint32_t hash)
{
    auto iter = yscLoader.find(hash);
    if (iter != yscLoader.end())
    {
        iter->second->RefCount++;
        return iter->second.get();
    }
    else
    {
        YscLoader* loader = new YscLoader();
        addToWaitingList(GlobalPool::GetInstance()->resourcesPool.create(ysc, hash, loader));
        loader->RefCount++;
        yscLoader.insert({hash, std::unique_ptr<YscLoader>(loader)});
        return loader;
    }
}

inline void ResourceManager::addToMainQueue(Resource* res)
{
    std::lock_guard<std::mutex> lock(gameworld->resources_lock);
    gameworld->resources.push_back(res);
}

void ResourceManager::addToWaitingList(Resource* res)
{
    std::lock_guard<std::mutex> lock(mylock);
    waitingList.push_back(res);
    loadCondition.notify_one();
}

inline Resource* ResourceManager::removeFromWaitingList()
{
    std::unique_lock<std::mutex> lock(mylock);

    loadCondition.wait(lock, [this] { return !waitingList.empty(); });

    Resource* res = waitingList.front();
    waitingList.pop_front();
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
                auto it = gameworld->getGameData()->Entries[res->type].find(res->Hash);
                if (it != gameworld->getGameData()->Entries[res->type].end())
                {
                    uint8_t* allocatedMemory = nullptr;

                    while (!allocatedMemory)
                    {
                        allocatedMemory = (uint8_t*)resource_allocator->allocate(it->second->UncompressedFileSize, 16);
                        //if (!allocatedMemory)
                        //printf("TRYING AGAIN\n");
                    }

                    //printf("DONE\n");

                    res->Buffer = allocatedMemory;
                    res->BufferSize = it->second->UncompressedFileSize;
                    gameworld->getGameData()->extractFileResource(*(it->second), res->Buffer, res->BufferSize);
                    res->SystemSize = (it->second->SystemSize);
                }
                if (res->type == ymap)
                {
                    YmapLoader* iter = static_cast<YmapLoader*>(res->file);
                    memstream stream(&res->Buffer[0], res->BufferSize);
                    iter->Init(stream);

                    for (auto& mlo : iter->CMloInstanceDefs)
                    {
                        auto it = gameworld->getGameData()->MloDictionary.find(mlo.fwEntityDef.archetypeName);
                        if (it != gameworld->getGameData()->MloDictionary.end())
                        {
                            for (auto& EntityDef : it->second)
                            {
                                glm::quat rotmultiply = EntityDef.rotation * mlo.fwEntityDef.rotation;
                                rotmultiply.w = -rotmultiply.w;
                                glm::mat4 matrix = glm::translate(glm::mat4(1.0f), mlo.fwEntityDef.position + EntityDef.position) *
                                                   glm::mat4_cast(glm::quat(-mlo.fwEntityDef.rotation.w, -mlo.fwEntityDef.rotation.x,
                                                                            -mlo.fwEntityDef.rotation.y, -mlo.fwEntityDef.rotation.z)) *
                                                   glm::scale(glm::mat4(1.0f),
                                                              glm::vec3(EntityDef.scaleXY, EntityDef.scaleXY, EntityDef.scaleZ));
                                EntityDef.position = mlo.fwEntityDef.position + EntityDef.position;
                                EntityDef.rotation = rotmultiply;
                                iter->Objects.emplace_back(EntityDef);
                            }
                        }
                    }

                    for (auto& object : iter->Objects)
                    {
                        std::unordered_map<uint32_t, fwArchetype*>::iterator it = gameworld->getGameData()->Archetypes.find(object.EntityDef.archetypeName);
                        if (it != gameworld->getGameData()->Archetypes.end())
                        {
                            if (it->second->GetType() == 2)
                            {
                                printf("");
                            }

                            object.archetype = it->second;

                            object.BoundPos = object.EntityDef.position - object.archetype->BaseArchetypeDef.bsCentre;
                            object.BoundRadius = object.archetype->BaseArchetypeDef
                                                     .bsRadius;  //* std::max(object.CEntity.scaleXY, object.CEntity.scaleZ); TREES doesnt render with multiplying by scale

                            if (object.EntityDef.lodDist <= 0)
                                object.EntityDef.lodDist = it->second->BaseArchetypeDef.lodDist;
                            if (object.EntityDef.childLodDist <= 0)
                                object.EntityDef.childLodDist = it->second->BaseArchetypeDef.lodDist;
                        }
                        else
                        {
                            //	printf("ERROR\n"); ACTUALLY IT CAN HAPPEN
                            object.archetype = nullptr;
                            object.EntityDef.lodDist = 0.f;  //	HACK = DONT RENDER OBJECTS WITH UNKNOWN ARCHETYPE
                        }

                        object.EntityDef.lodDist *= object.EntityDef.lodDist;            // glm::length2
                        object.EntityDef.childLodDist *= object.EntityDef.childLodDist;  // glm::length2
                    }
                }
                addToMainQueue(res);
            }
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

void ResourceManager::RemoveAll()
{
    for (auto it = ytdLoader.begin(); it != ytdLoader.end();)
    {
        TextureMemory -= it->second->gpuMemory;
        it = ytdLoader.erase(it);
    }
}

void ResourceManager::UpdateResourceCache()
{
    //	printf("FREE SPACE %zd\n", _main_allocator->getSize() - _main_allocator->getUsedMemory());
    // REMOVE OBJECTS WHEN WE ARE IN ANOTHER CELL????  RUN GARBAGE COLLECTOR WHEN IN ANOTHER CEL
    for (auto it = ybnLoader.begin(); it != ybnLoader.end();)
    {
        if ((it->second)->RefCount == 0 && (it->second)->Loaded)
        {
            gameworld->getPhysicsSystem()->removeRigidBody((it->second)->getRigidBody());
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
        if ((it->second)->RefCount == 0 && (it->second)->Loaded)
        {
            GlobalPool::GetInstance()->ymapPool.remove(it->second);
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
            GlobalPool::GetInstance()->ydrPool.remove(it->second);
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
        if ((it->second)->RefCount == 0 && (it->second)->Loaded)
        {
            GlobalGpuMemory -= it->second->gpuMemory;
            GlobalPool::GetInstance()->yftPool.remove(it->second);
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
            GlobalPool::GetInstance()->ytdPool.remove(it->second);
            it = ytdLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
