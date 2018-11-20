#include "GameWorld.h"
#include "ResourceManager.h"
#include "RpfEntry.h"

#include "Object.h"
#include "YbnLoader.h"
#include "YddLoader.h"
#include "YftLoader.h"
#include "YmapLoader.h"
#include "YndLoader.h"
#include "YtdLoader.h"
#include "YtypLoader.h"

#include "GlobalPool.h"

ResourceManager::ResourceManager(GameWorld* world) : gameworld{world}, running(true) {
    // printf("RESOURCE MANAGER LOADED!\n");
    ResourcesThread = std::thread(&ResourceManager::update, this);
    ResourcesThread.detach();

    // yndLoader.reserve(500);
    // ytypLoader.reserve(500);
    ydrLoader.reserve(500);
    yddLoader.reserve(500);
    yftLoader.reserve(500);
    ytdLoader.reserve(1500);
    ybnLoader.reserve(50);
    ymapLoader.reserve(500);
}

ResourceManager::~ResourceManager() {
    /*{
        std::unique_lock<std::mutex> lock(mylock);
        running = false;
        loadCondition.notify_one();
    }
    ResourcesThread.join();*/
}

void ResourceManager::GetGtxd(uint32_t hash) {
    /* auto iter = data.GtxdEntries.find(hash);
     if (iter != data.GtxdEntries.end()) {
         auto it = ytdLoader.find(iter->second);
         if (it == ytdLoader.end()) {
             ytdLoader[iter->second] = myNew YtdLoader();
             AddToWaitingList(Resource(ytd, iter->second, 0, nullptr));
         }
     }*/
}

YmapLoader* ResourceManager::GetYmap(uint32_t hash) {
    auto it = ymapLoader.find(hash);
    if (it != ymapLoader.end()) {
        it->second->RefCount++;
        return it->second;
    } else {
        YmapLoader* loader = GlobalPool::getInstance().YmapPool.Load();
        AddToWaitingList(Resource(ymap, hash, 0, loader));
        loader->RefCount++;
        ymapLoader.insert({hash, loader});

        return loader;
    }
}

YdrLoader* ResourceManager::GetYdr(uint32_t hash, uint32_t TextureDictionaryHash) {
    auto iter = ydrLoader.find(hash);
    if (iter != ydrLoader.end()) {
        iter->second->RefCount++;
        return iter->second;
    } else {
        YdrLoader* loader = GlobalPool::getInstance().YdrPool.Load();
        AddToWaitingList(Resource(ydr, hash, TextureDictionaryHash, loader));
        loader->RefCount++;
        ydrLoader.insert({hash, loader});

        return loader;
    }
}

YtdLoader* ResourceManager::GetYtd(uint32_t hash) {
    auto it = ytdLoader.find(hash);
    if (it != ytdLoader.end()) {
        it->second->RefCount++;
        return it->second;
    } else {
        // LoadGtxd(hash);
        YtdLoader* loader = GlobalPool::getInstance().YtdPool.Load();
        AddToWaitingList(Resource(ytd, hash, 0, loader));
        loader->RefCount++;
        ytdLoader.insert({hash, loader});

        return loader;
    }
}

YddLoader* ResourceManager::GetYdd(uint32_t hash, uint32_t TextureDictionaryHash) {
    auto iter = yddLoader.find(hash);
    if (iter != yddLoader.end()) {
        iter->second->RefCount++;
        return iter->second;
    } else {
        YddLoader* loader = GlobalPool::getInstance().YddPool.Load();
        AddToWaitingList(Resource(ydd, hash, TextureDictionaryHash, loader));
        loader->RefCount++;
        yddLoader.insert({hash, loader});

        return loader;
    }
}

YftLoader* ResourceManager::GetYft(uint32_t hash, uint32_t TextureDictionaryHash) {
    auto iter = yftLoader.find(hash);
    if (iter != yftLoader.end()) {
        iter->second->RefCount++;
        return iter->second;
    } else {
        YftLoader* loader = GlobalPool::getInstance().YftPool.Load();
        AddToWaitingList(Resource(yft, hash, TextureDictionaryHash, loader));
        loader->RefCount++;
        yftLoader.insert({hash, loader});

        return loader;
    }
}

YbnLoader* ResourceManager::GetYbn(uint32_t hash) {
    std::unordered_map<uint32_t, YbnLoader*>::iterator iter = ybnLoader.find(hash);
    if (iter != ybnLoader.end()) {
        iter->second->RefCount++;
        return iter->second;
    } else {
        YbnLoader* loader = GlobalPool::getInstance().YbnPool.Load();
        AddToWaitingList(Resource(ybn, hash, 0, loader));
        loader->RefCount++;
        ybnLoader.insert({hash, loader});
        return loader;
    }
}

void ResourceManager::AddToWaitingList(Resource& res) {
    std::lock_guard<std::mutex> lock(mylock);
    waitingList.push_back(res);
    loadCondition.notify_one();
}

void ResourceManager::LoadDrawable(RpfResourceFileEntry* entry, Resource& res) {
    res.Buffer.resize(entry->SystemSize + entry->GraphicsSize);
    gameworld->getGameData()->ExtractFileResource(*(entry), res.Buffer);
    res.SystemSize = entry->SystemSize;

    auto it = gameworld->getGameData()->YtdEntries.find(res.TextureDictionaryHash);
    if (it == gameworld->getGameData()->YtdEntries.end()) {
        res.TextureDictionaryHash = 0;
    }
}

inline void ResourceManager::AddToMainQueue(Resource& res) {
    gameworld->resources_lock.lock();
    gameworld->resources.push_back(std::move(res));
    gameworld->resources_lock.unlock();
}

void ResourceManager::update() {
    while (running) {
        std::unique_lock<std::mutex> lock(mylock);

        loadCondition.wait(lock, [this] { return !waitingList.empty(); });

        auto res = std::move(waitingList.back());
        waitingList.pop_back();
        lock.unlock();

        switch (res.type) {
        case ymap: {
            auto it = gameworld->getGameData()->YmapEntries.find(res.Hash);
            if (it != gameworld->getGameData()->YmapEntries.end()) {
                res.Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
                gameworld->getGameData()->ExtractFileResource(*(it->second), res.Buffer);

                YmapLoader* iter = static_cast<YmapLoader*>(res.file);

                memstream stream(res.Buffer.data(), res.Buffer.size());
                iter->Init(stream);

                for (auto& object : *iter->Objects) {
                    std::unordered_map<uint32_t, Archetype*>::iterator it =
                        gameworld->getGameData()->Archetypes.find(object.CEntity.archetypeName);
                    if (it != gameworld->getGameData()->Archetypes.end()) {
                        object.Archetype = it->second;

                        object.BoundPos =
                            object.CEntity.position - object.Archetype->BaseArchetypeDef.bsCentre;
                        object.BoundRadius =
                            object.Archetype->BaseArchetypeDef.bsRadius *
                            std::max(object.CEntity.scaleXY, object.CEntity.scaleZ);

                        if (object.CEntity.lodDist <= 0)
                            object.CEntity.lodDist = it->second->BaseArchetypeDef.lodDist;
                        if (object.CEntity.childLodDist <= 0)
                            object.CEntity.childLodDist = it->second->BaseArchetypeDef.lodDist;
                    } else {
                        printf("ERROR\n");
                    }

                    object.CEntity.lodDist *= object.CEntity.lodDist;           // glm::length2
                    object.CEntity.childLodDist *= object.CEntity.childLodDist; // glm::length2
                }
            }
            AddToMainQueue(res);
            break;
        }
        case ydr: {
            auto it = gameworld->getGameData()->YdrEntries.find(res.Hash);
            if (it != gameworld->getGameData()->YdrEntries.end()) {
                LoadDrawable(it->second, res);
            }
            AddToMainQueue(res);
            break;
        }
        case ydd: {
            auto it = gameworld->getGameData()->YddEntries.find(res.Hash);
            if (it != gameworld->getGameData()->YddEntries.end()) {
                LoadDrawable(it->second, res);
            }
            AddToMainQueue(res);
            break;
        }
        case yft: {
            auto it = gameworld->getGameData()->YftEntries.find(res.Hash);
            if (it != gameworld->getGameData()->YftEntries.end()) {
                LoadDrawable(it->second, res);
            }
            AddToMainQueue(res);
            break;
        }
        case ytd: {
            auto it = gameworld->getGameData()->YtdEntries.find(res.Hash);
            if (it != gameworld->getGameData()->YtdEntries.end()) {
                res.Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
                gameworld->getGameData()->ExtractFileResource(*(it->second), res.Buffer);
                res.SystemSize = it->second->SystemSize;
            }
            AddToMainQueue(res);
            break;
        }
        case ybn: {
            auto it = gameworld->getGameData()->YbnEntries.find(res.Hash);
            if (it != gameworld->getGameData()->YbnEntries.end()) {
                res.Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
                gameworld->getGameData()->ExtractFileResource(*(it->second), res.Buffer);
                res.SystemSize = it->second->SystemSize;

                memstream stream(res.Buffer.data(), res.Buffer.size());
                res.file->Init(stream);
            }
            AddToMainQueue(res);
            break;
        }
        }
    }
}

void ResourceManager::UpdateResourceCache() {
    // REMOVE OBJECTS WHEN WE ARE IN ANOTHER CELL????  RUN GARBAGE COLLECTOR WHEN IN ANOTHER CEL
    for (auto it = ybnLoader.begin(); it != ybnLoader.end();) {
        if ((it->second)->RefCount == 0 && (it->second)->Loaded) {
            GlobalPool::getInstance().YbnPool.Remove(it->second);
            it = ybnLoader.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = ymapLoader.begin(); it != ymapLoader.end();) {
        if ((it->second)->RefCount == 0 && (it->second)->Loaded) {
            // YmapPool::getPool().Remove(it->second, dynamicsWorld);
            GlobalPool::getInstance().YmapPool.Remove(it->second);
            it = ymapLoader.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = ydrLoader.begin(); it != ydrLoader.end();) {
        if ((it->second)->RefCount == 0 && (it->second)->Loaded) {
            GlobalGpuMemory -= it->second->gpuMemory;
            GlobalPool::getInstance().YdrPool.Remove(it->second);
            it = ydrLoader.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = yddLoader.begin(); it != yddLoader.end();) {
        if ((it->second)->RefCount == 0 && (it->second)->Loaded) {
            GlobalGpuMemory -= it->second->gpuMemory;
            GlobalPool::getInstance().YddPool.Remove(it->second);
            it = yddLoader.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = yftLoader.begin(); it != yftLoader.end();) {
        if ((it->second)->RefCount == 0 && (it->second)->Loaded) {
            GlobalGpuMemory -= it->second->gpuMemory;
            GlobalPool::getInstance().YftPool.Remove(it->second);
            it = yftLoader.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = ytdLoader.begin(); it != ytdLoader.end();) {
        if ((it->second)->RefCount == 0 && (it->second)->Loaded) {
            TextureMemory -= it->second->gpuMemory;
            GlobalPool::getInstance().YtdPool.Remove(it->second);
            it = ytdLoader.erase(it);
        } else {
            ++it;
        }
    }
}
