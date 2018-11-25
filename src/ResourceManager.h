#pragma once

#include <mutex>
#include <thread>
#include <unordered_map>
#include "includes.h"
#include "FileType.h"
#include <deque>

class GameWorld;
class RpfEntry;

class RpfResourceFileEntry;

class YdrLoader;
class YddLoader;
class YftLoader;
class YtdLoader;
class YbnLoader;
class YmapLoader;

class ResourceManager {
    std::thread ResourcesThread;
    std::mutex mylock;
    std::condition_variable loadCondition;
    bool running;

    std::deque<Resource*> waitingList;
    GameWorld* gameworld;

public:
    std::unordered_map<uint32_t, YdrLoader*> ydrLoader;
    std::unordered_map<uint32_t, YddLoader*> yddLoader;
    std::unordered_map<uint32_t, YftLoader*> yftLoader;
    std::unordered_map<uint32_t, YtdLoader*> ytdLoader;
    std::unordered_map<uint32_t, YbnLoader*> ybnLoader;
    std::unordered_map<uint32_t, YmapLoader*> ymapLoader;

    ResourceManager(GameWorld* world);
    ~ResourceManager();

    uint64_t GlobalGpuMemory = 0;
    uint64_t TextureMemory = 0;

    void GetGtxd(uint32_t hash);
    // GetFile<YdrLoader, Type::ydr>(uint32_t hash, uint32_t TextureDictionaryHash);
    YmapLoader* GetYmap(uint32_t hash);
    YdrLoader* GetYdr(uint32_t hash);
    YtdLoader* GetYtd(uint32_t hash);
    YddLoader* GetYdd(uint32_t hash);
    YftLoader* GetYft(uint32_t hash);
    YbnLoader* GetYbn(uint32_t hash);

    void Load(RpfResourceFileEntry* entry, Resource* res);
    inline void AddToMainQueue(Resource* res);
    void AddToWaitingList(Resource* res);

    void update();

    void UpdateResourceCache();
};
