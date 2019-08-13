#pragma once

#include <condition_variable>
#include <queue>
#include <mutex>
#include <thread>
#include <unordered_map>
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
    std::thread resourcesThread;
    std::mutex mylock;
    std::condition_variable loadCondition;
    bool running;

    std::queue<Resource*> waitingList;
    GameWorld* gameworld;

    std::unordered_map<uint32_t, YdrLoader*> ydrLoader;
    std::unordered_map<uint32_t, YddLoader*> yddLoader;
    std::unordered_map<uint32_t, YftLoader*> yftLoader;
    std::unordered_map<uint32_t, YtdLoader*> ytdLoader;
    std::unordered_map<uint32_t, YbnLoader*> ybnLoader;
    std::unordered_map<uint32_t, YmapLoader*> ymapLoader;
    std::unordered_map<uint32_t, std::unique_ptr<YscLoader>> yscLoader;

public:
    ResourceManager(GameWorld* world);
    ~ResourceManager();

    uint64_t GlobalGpuMemory = 0;
    uint64_t TextureMemory = 0;

    std::unique_ptr<ThreadSafeAllocator> resource_allocator;
    void getGtxd(uint32_t hash);
    // GetFile<YdrLoader, Type::ydr>(uint32_t hash, uint32_t TextureDictionaryHash);
    YmapLoader* getYmap(uint32_t hash);
    YdrLoader* getYdr(uint32_t hash);
    YtdLoader* getYtd(uint32_t hash);
    YddLoader* getYdd(uint32_t hash);
    YftLoader* getYft(uint32_t hash);
    YbnLoader* getYbn(uint32_t hash);
    YscLoader* getYsc(uint32_t hash);

    inline void addToMainQueue(Resource* res);

    Resource* removeFromWaitingList();
    void addToWaitingList(Resource* res);

    void update();

    void removeAll();

    void updateResourceCache();
};
