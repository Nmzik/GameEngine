#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <unordered_map>
#include "FileType.h"
#include "ThreadSafeAllocator.h"
#include "includes.h"

class GameWorld;

class RpfEntry;
class RpfResourceFileEntry;

class YdrLoader;
class YddLoader;
class YftLoader;
class YtdLoader;
class YbnLoader;
class YmapLoader;

class ResourceManager
{
    std::thread ResourcesThread;
    std::mutex mylock;
    std::condition_variable loadCondition;
    bool running;

    std::deque<Resource*> waitingList;
    GameWorld* gameworld;

    std::unordered_map<uint32_t, std::unique_ptr<YdrLoader>> ydrLoader;
    std::unordered_map<uint32_t, std::unique_ptr<YddLoader>> yddLoader;
    std::unordered_map<uint32_t, std::unique_ptr<YftLoader>> yftLoader;
    std::unordered_map<uint32_t, std::unique_ptr<YtdLoader>> ytdLoader;
    std::unordered_map<uint32_t, std::unique_ptr<YbnLoader>> ybnLoader;
    std::unordered_map<uint32_t, std::unique_ptr<YmapLoader>> ymapLoader;

public:
    ResourceManager(GameWorld* world);
    ~ResourceManager();

    uint64_t GlobalGpuMemory = 0;
    uint64_t TextureMemory = 0;

    void Initialize();

    ThreadSafeAllocator* resource_allocator;
    void GetGtxd(uint32_t hash);
    // GetFile<YdrLoader, Type::ydr>(uint32_t hash, uint32_t TextureDictionaryHash);
    YmapLoader* GetYmap(uint32_t hash);
    YdrLoader* GetYdr(uint32_t hash);
    YtdLoader* GetYtd(uint32_t hash);
    YddLoader* GetYdd(uint32_t hash);
    YftLoader* GetYft(uint32_t hash);
    YbnLoader* GetYbn(uint32_t hash);

    FileType* loadSync(Type type, uint32_t Hash);

    inline void addToMainQueue(Resource* res);
    void addToWaitingList(Resource* res);

    void update();

    void RemoveAll();

    void UpdateResourceCache();
};
