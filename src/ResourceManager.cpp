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

ResourceManager::ResourceManager(GameWorld* world)
	: gameworld{ world }
	, running(true)
{
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


	/*size_t memory_size = 1024ULL * 1024 * 1024; //1GB
	uint8_t* _memory = (uint8_t*)malloc(memory_size);

	if (_memory == nullptr)
	{
		printf("ERROR");
	}

	_main_allocator = new aqua::FreeListAllocator(memory_size - sizeof(aqua::FreeListAllocator), pointer_math::add(_memory, sizeof(aqua::FreeListAllocator)));
	uint8_t* test = (uint8_t*)_main_allocator->allocate(10 * 1024 * 1024, 16);*/
}

ResourceManager::~ResourceManager()
{
	/*{
	 std::unique_lock<std::mutex> lock(mylock);
	 running = false;
	 loadCondition.notify_one();
	}
	ResourcesThread.join();*/
}

void ResourceManager::GetGtxd(uint32_t hash)
{
	/* auto iter = data.GtxdEntries.find(hash);
	 if (iter != data.GtxdEntries.end()) {
	  auto it = ytdLoader.find(iter->second);
	  if (it == ytdLoader.end()) {
	   ytdLoader[iter->second] = myNew YtdLoader();
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
		YmapLoader* loader = GlobalPool::getInstance().YmapPool.Load();
		AddToWaitingList(new Resource(ymap, hash, loader));
		loader->RefCount++;
		ymapLoader.insert({ hash, loader });

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
		YdrLoader* loader = GlobalPool::getInstance().YdrPool.Load();
		AddToWaitingList(new Resource(ydr, hash, loader));
		loader->RefCount++;
		ydrLoader.insert({ hash, loader });

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
		// LoadGtxd(hash);
		YtdLoader* loader = GlobalPool::getInstance().YtdPool.Load();
		AddToWaitingList(new Resource(ytd, hash, loader));
		loader->RefCount++;
		ytdLoader.insert({ hash, loader });

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
		YddLoader* loader = GlobalPool::getInstance().YddPool.Load();
		AddToWaitingList(new Resource(ydd, hash, loader));
		loader->RefCount++;
		yddLoader.insert({ hash, loader });

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
		YftLoader* loader = GlobalPool::getInstance().YftPool.Load();
		AddToWaitingList(new Resource(yft, hash, loader));
		loader->RefCount++;
		yftLoader.insert({ hash, loader });

		return loader;
	}
}

YbnLoader* ResourceManager::GetYbn(uint32_t hash)
{
	std::unordered_map<uint32_t, YbnLoader*>::iterator iter = ybnLoader.find(hash);
	if (iter != ybnLoader.end())
	{
		iter->second->RefCount++;
		return iter->second;
	}
	else
	{
		YbnLoader* loader = GlobalPool::getInstance().YbnPool.Load();
		AddToWaitingList(new Resource(ybn, hash, loader));
		loader->RefCount++;
		ybnLoader.insert({ hash, loader });
		return loader;
	}
}

void ResourceManager::AddToWaitingList(Resource* res)
{
	std::lock_guard<std::mutex> lock(mylock);
	waitingList.push_back(res);
	loadCondition.notify_one();
}

inline void ResourceManager::AddToMainQueue(Resource* res)
{
	gameworld->resources_lock.lock();
	gameworld->resources.push_back(res);
	gameworld->resources_lock.unlock();
}

void ResourceManager::update()
{
	while (running)
	{
		std::unique_lock<std::mutex> lock(mylock);

		loadCondition.wait(lock, [this] { return !waitingList.empty(); });

		auto res = waitingList.front();
		waitingList.pop_front();
		lock.unlock();

		switch (res->type)
		{
			case ymap:
			case ydr:
			case ydd:
			case yft:
			case ytd:
			case ybn:
			{
				auto it = gameworld->getGameData()->Entries[res->type].find(res->Hash);
				if (it != gameworld->getGameData()->Entries[res->type].end())
				{
					res->Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);
					res->SystemSize = (it->second->SystemSize);
				}
				AddToMainQueue(res);
				break;
			}
		}
	}
}

void ResourceManager::RemoveAll()
{
	for (auto it = ybnLoader.begin(); it != ybnLoader.end();)
	{
			GlobalPool::getInstance().YbnPool.Remove(it->second);
			it = ybnLoader.erase(it);
	}

	for (auto it = ymapLoader.begin(); it != ymapLoader.end();)
	{
			GlobalPool::getInstance().YmapPool.Remove(it->second);
			it = ymapLoader.erase(it);
	}
	for (auto it = ydrLoader.begin(); it != ydrLoader.end();)
	{
			GlobalGpuMemory -= it->second->gpuMemory;
			GlobalPool::getInstance().YdrPool.Remove(it->second);
			it = ydrLoader.erase(it);
	}

	for (auto it = yddLoader.begin(); it != yddLoader.end();)
	{
			GlobalGpuMemory -= it->second->gpuMemory;
			GlobalPool::getInstance().YddPool.Remove(it->second);
			it = yddLoader.erase(it);
	}

	for (auto it = yftLoader.begin(); it != yftLoader.end();)
	{
			GlobalGpuMemory -= it->second->gpuMemory;
			GlobalPool::getInstance().YftPool.Remove(it->second);
			it = yftLoader.erase(it);
	}

	for (auto it = ytdLoader.begin(); it != ytdLoader.end();)
	{
			TextureMemory -= it->second->gpuMemory;
			GlobalPool::getInstance().YtdPool.Remove(it->second);
			it = ytdLoader.erase(it);
	}
}

void ResourceManager::UpdateResourceCache()
{
	// REMOVE OBJECTS WHEN WE ARE IN ANOTHER CELL????  RUN GARBAGE COLLECTOR WHEN IN ANOTHER CEL
	for (auto it = ybnLoader.begin(); it != ybnLoader.end();)
	{
		if ((it->second)->RefCount == 0 && (it->second)->Loaded)
		{
			GlobalPool::getInstance().YbnPool.Remove(it->second);
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
			GlobalPool::getInstance().YmapPool.Remove(it->second);
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
			GlobalPool::getInstance().YdrPool.Remove(it->second);
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
			GlobalPool::getInstance().YddPool.Remove(it->second);
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
			GlobalPool::getInstance().YftPool.Remove(it->second);
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
			GlobalPool::getInstance().YtdPool.Remove(it->second);
			it = ytdLoader.erase(it);
		}
		else
		{
			++it;
		}
	}
}
