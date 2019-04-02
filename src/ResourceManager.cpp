#include "ResourceManager.h"
#include "GameWorld.h"
#include "RpfEntry.h"

#include "Object.h"
#include "YbnLoader.h"
#include "YddLoader.h"
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
    Resource* exitResource = new Resource(Type::null, 0, nullptr);
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
        return it->second.get();
    }
    else
    {
        YmapLoader* loader = new YmapLoader();
        addToWaitingList(new Resource(ymap, hash, loader));
        loader->RefCount++;
        ymapLoader.insert({hash, std::unique_ptr<YmapLoader>(loader)});

        return loader;
    }
}

YdrLoader* ResourceManager::GetYdr(uint32_t hash)
{
    auto iter = ydrLoader.find(hash);
    if (iter != ydrLoader.end())
    {
        iter->second->RefCount++;
        return iter->second.get();
    }
    else
    {
        YdrLoader* loader = new YdrLoader();
        addToWaitingList(new Resource(ydr, hash, loader));
        loader->RefCount++;
        ydrLoader.insert({hash, std::unique_ptr<YdrLoader>(loader)});

        return loader;
    }
}

YtdLoader* ResourceManager::GetYtd(uint32_t hash)
{
    auto it = ytdLoader.find(hash);
    if (it != ytdLoader.end())
    {
        it->second->RefCount++;
        return it->second.get();
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
                    return it->second.get();
                }
                else
                {
                    YtdLoader* loader = new YtdLoader();
                    addToWaitingList(new Resource(ytd, iter->second, loader));
                    loader->RefCount++;
                    ytdLoader.insert({iter->second, std::unique_ptr<YtdLoader>(loader)});

                    return loader;
                }
            }
            else
            {
                printf("HD Texture not found\n");
            }
        }

        YtdLoader* loader = new YtdLoader();
        addToWaitingList(new Resource(ytd, hash, loader));
        loader->RefCount++;
        ytdLoader.insert({hash, std::unique_ptr<YtdLoader>(loader)});

        return loader;
    }
}

YddLoader* ResourceManager::GetYdd(uint32_t hash)
{
    auto iter = yddLoader.find(hash);
    if (iter != yddLoader.end())
    {
        iter->second->RefCount++;
        return iter->second.get();
    }
    else
    {
        YddLoader* loader = new YddLoader();
        addToWaitingList(new Resource(ydd, hash, loader));
        loader->RefCount++;
        yddLoader.insert({hash, std::unique_ptr<YddLoader>(loader)});

        return loader;
    }
}

YftLoader* ResourceManager::GetYft(uint32_t hash)
{
    auto iter = yftLoader.find(hash);
    if (iter != yftLoader.end())
    {
        iter->second->RefCount++;
        return iter->second.get();
    }
    else
    {
        YftLoader* loader = new YftLoader();
        addToWaitingList(new Resource(yft, hash, loader));
        loader->RefCount++;
        yftLoader.insert({hash, std::unique_ptr<YftLoader>(loader)});

        return loader;
    }
}

YbnLoader* ResourceManager::GetYbn(uint32_t hash)
{
    auto iter = ybnLoader.find(hash);
    if (iter != ybnLoader.end())
    {
        iter->second->RefCount++;
        return iter->second.get();
    }
    else
    {
        YbnLoader* loader = new YbnLoader();
        addToWaitingList(new Resource(ybn, hash, loader));
        loader->RefCount++;
        ybnLoader.insert({hash, std::unique_ptr<YbnLoader>(loader)});
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
        addToWaitingList(new Resource(ysc, hash, loader));
        loader->RefCount++;
        yscLoader.insert({hash, std::unique_ptr<YscLoader>(loader)});
        return loader;
    }
}

void ResourceManager::addToWaitingList(Resource* res)
{
    std::lock_guard<std::mutex> lock(mylock);
    waitingList.push_back(res);
    loadCondition.notify_one();
}

inline void ResourceManager::addToMainQueue(Resource* res)
{
    std::lock_guard<std::mutex> lock(gameworld->resources_lock);
    gameworld->resources.push_back(res);
}

void ResourceManager::update()
{
    while (running)
    {
        std::unique_lock<std::mutex> lock(mylock);

        loadCondition.wait(lock, [this] { return !waitingList.empty(); });

        Resource* res = waitingList.front();
        waitingList.pop_front();
        lock.unlock();

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
            it = ytdLoader.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
