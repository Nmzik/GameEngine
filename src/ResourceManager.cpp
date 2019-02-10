#include "ResourceManager.h"
#include "GameWorld.h"
#include "RpfEntry.h"

#include "Object.h"
#include "YbnLoader.h"
#include "YddLoader.h"
#include "YftLoader.h"
#include "YmapLoader.h"
#include "YndLoader.h"
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

	uint8_t* _memory   = new uint8_t[50 * 1024 * 1024]; // 50MB
	resource_allocator = new ThreadSafeAllocator(50 * 1024 * 1024, _memory);

	ResourcesThread = std::thread(&ResourceManager::update, this);
	//ResourcesThread.detach();
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
		AddToWaitingList(new Resource(ymap, hash, loader));
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
		AddToWaitingList(new Resource(ydr, hash, loader));
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
					AddToWaitingList(new Resource(ytd, iter->second, loader));
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
		AddToWaitingList(new Resource(ytd, hash, loader));
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
		AddToWaitingList(new Resource(ydd, hash, loader));
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
		AddToWaitingList(new Resource(yft, hash, loader));
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
		AddToWaitingList(new Resource(ybn, hash, loader));
		loader->RefCount++;
		ybnLoader.insert({hash, std::unique_ptr<YbnLoader>(loader)});
		return loader;
	}
}

FileType* ResourceManager::loadSync(Type type, uint32_t Hash)
{
	auto it = gameworld->getGameData()->Entries[type].find(Hash);
	if (it != gameworld->getGameData()->Entries[type].end())
	{
		/*std::vector<uint8_t> Buffer;
		Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
		gameworld->getGameData()->ExtractFileResource(*(it->second), Buffer);

		memstream stream(Buffer.data(), Buffer.size());

		switch (type)
		{
		case ydr:
		 break;
		case ydd:
		 break;
		case yft:
		 break;
		case ytd:
		 break;
		case ybn: {
		 YbnLoader* loader = new YbnLoader();
		 loader->Init(stream);
		 return loader;
		}
		case ymap:
		 break;
		case ynd:
		 break;
		case ynv:
		 break;
		default:
		 break;
		}*/
	}
	return nullptr;
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

		Resource* res = waitingList.front();
		waitingList.pop_front();
		lock.unlock();

		auto it = gameworld->getGameData()->Entries[res->type].find(res->Hash);
		if (it != gameworld->getGameData()->Entries[res->type].end())
		{
			uint8_t* allocatedMemory = (uint8_t*)resource_allocator->allocate(it->second->UncompressedFileSize, 16);

			if (allocatedMemory != nullptr)
			{
				res->Buffer     = allocatedMemory;
				res->BufferSize = it->second->UncompressedFileSize;
				gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer, res->BufferSize);
				res->SystemSize = (it->second->SystemSize);
			}
			else
			{
				//	printf("NOT ENOUGHT MEMORY IN POOL - WILL TRY NEXT TIME\n");

				lock.lock();
				waitingList.push_front(res);
				lock.unlock();

				continue;
			}
		}
		AddToMainQueue(res);
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
