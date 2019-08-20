#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include "GameData.h"
#include "ThreadSafeAllocator.h"
#include "common.h"

class GameWorld;

class RpfEntry;
class RpfResourceFileEntry;

class YdrLoader;
class YddLoader;
class YftLoader;
class YtdLoader;
class YbnLoader;
class YmapLoader;
class YscLoader;

class ResourceManager
{
    std::thread loadThread;
    std::mutex loadThreadLock;
    std::condition_variable loadCondition;
    bool running;

    std::queue<Resource*> waitingList;
    GameData& data;

    std::unordered_map<uint32_t, YdrLoader*> ydrLoader;
    std::unordered_map<uint32_t, YddLoader*> yddLoader;
    std::unordered_map<uint32_t, YftLoader*> yftLoader;
    std::unordered_map<uint32_t, YtdLoader*> ytdLoader;
    std::unordered_map<uint32_t, YbnLoader*> ybnLoader;
    std::unordered_map<uint32_t, YmapLoader*> ymapLoader;
    std::unordered_map<uint32_t, std::unique_ptr<YscLoader>> yscLoader;

    void update();

    inline void addToMainQueue(Resource* res);

    Resource* removeFromWaitingList();

    void getGtxd(uint32_t hash);

public:
    ResourceManager(GameData* gameData);
    ~ResourceManager();

    uint64_t GlobalGpuMemory = 0;
    uint64_t TextureMemory = 0;

    std::mutex mainThreadLock;
    std::queue<Resource*> mainThreadResources;
    std::queue<Resource*> tempMainThreadResources;

    std::unique_ptr<ThreadSafeAllocator> resource_allocator;
    // GetFile<YdrLoader, Type::ydr>(uint32_t hash, uint32_t TextureDictionaryHash);
    YmapLoader* getYmap(uint32_t hash);
    YdrLoader* getYdr(uint32_t hash);
    YtdLoader* getYtd(uint32_t hash);
    YddLoader* getYdd(uint32_t hash);
    YftLoader* getYft(uint32_t hash);
    YbnLoader* getYbn(uint32_t hash);
    YscLoader* getYsc(uint32_t hash);

    GameData* getGameData() const
    {
        return &data;
    }

    void addToWaitingList(Resource* res);
    void removeAll();
    void updateResourceCache(GameWorld* world);
};
