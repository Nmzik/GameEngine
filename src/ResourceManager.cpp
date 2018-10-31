#include "ResourceManager.h"
#include "GameWorld.h"
#include "RpfEntry.h"

#include "YmapLoader.h"
#include "Object.h"

ResourceManager::ResourceManager(GameWorld *world) : gameworld{ world }, running(true)
{
	//printf("RESOURCE MANAGER LOADED!\n");
	ResourcesThread = std::thread(&ResourceManager::update, this);
	ResourcesThread.detach();
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
	if (it == gameworld->getGameData()->YtdEntries.end())
	{
		res.TextureDictionaryHash = 0;
	}
}

inline void ResourceManager::AddToMainQueue(Resource & res)
{
	gameworld->resources_lock.lock();
	gameworld->resources.push_back(std::move(res));
	gameworld->resources_lock.unlock();
}

void ResourceManager::update()
{
	while (running) {
		std::unique_lock<std::mutex> lock(mylock);

		loadCondition.wait(lock, [this] {
			return !waitingList.empty();
		});

		auto res = std::move(waitingList.back());
		waitingList.pop_back();
		lock.unlock();

		switch (res.type) {
			case ymap: {
				auto it = gameworld->getGameData()->YmapEntries.find(res.Hash);
				if (it != gameworld->getGameData()->YmapEntries.end())
				{
					res.Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res.Buffer);

					YmapLoader* iter = static_cast<YmapLoader*>(res.file);

					memstream2 stream(res.Buffer.data(), res.Buffer.size());
					iter->Init(stream);

					for (auto& object : *iter->Objects)
					{
						std::unordered_map<uint32_t, Archetype*>::iterator it = gameworld->getGameData()->Archetypes.find(object.CEntity.archetypeName);
						if (it != gameworld->getGameData()->Archetypes.end())
						{
							object.Archetype = it->second;

							object.BoundPos = object.CEntity.position - object.Archetype->BaseArchetypeDef.bsCentre;
							object.BoundRadius = object.Archetype->BaseArchetypeDef.bsRadius * std::max(object.CEntity.scaleXY, object.CEntity.scaleZ);

							if (object.CEntity.lodDist <= 0) object.CEntity.lodDist = it->second->BaseArchetypeDef.lodDist;
							if (object.CEntity.childLodDist <= 0) object.CEntity.childLodDist = it->second->BaseArchetypeDef.lodDist;
						}
						else {
							printf("ERROR\n");
						}

						object.CEntity.lodDist *= object.CEntity.lodDist; //glm::length2
						object.CEntity.childLodDist *= object.CEntity.childLodDist; //glm::length2

					}
				}
				AddToMainQueue(res);
				break;
			}
			case ydr:
			{
				auto it = gameworld->getGameData()->YdrEntries.find(res.Hash);
				if (it != gameworld->getGameData()->YdrEntries.end())
				{
					LoadDrawable(it->second, res);
				}
				AddToMainQueue(res);
				break;
			}
			case ydd:
			{
				auto it = gameworld->getGameData()->YddEntries.find(res.Hash);
				if (it != gameworld->getGameData()->YddEntries.end())
				{
					LoadDrawable(it->second, res);
				}
				AddToMainQueue(res);
				break;
			}
			case yft:
			{
				auto it = gameworld->getGameData()->YftEntries.find(res.Hash);
				if (it != gameworld->getGameData()->YftEntries.end())
				{
					LoadDrawable(it->second, res);
				}
				AddToMainQueue(res);
				break;
			}
			case ytd:
			{
				auto it = gameworld->getGameData()->YtdEntries.find(res.Hash);
				if (it != gameworld->getGameData()->YtdEntries.end())
				{
					res.Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res.Buffer);
					res.SystemSize = it->second->SystemSize;
				}
				AddToMainQueue(res);
				break;
			}
			case ybn:
			{
				auto it = gameworld->getGameData()->YbnEntries.find(res.Hash);
				if (it != gameworld->getGameData()->YbnEntries.end())
				{
					res.Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res.Buffer);
					res.SystemSize = it->second->SystemSize;

					memstream2 stream(res.Buffer.data(), res.Buffer.size());
					res.file->Init(stream);
				}
				AddToMainQueue(res);
				break;
			}
		}
	}
}
