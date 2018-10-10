#include "ResourceManager.h"
#include "GameWorld.h"
#include "RpfEntry.h"

#include "YmapLoader.h"
#include "Object.h"

ResourceManager::ResourceManager(GameWorld *world) : gameworld{ world }
{
	//printf("RESOURCE MANAGER LOADED!\n");
	ResourcesThread = std::thread(&ResourceManager::update, this);
	ResourcesThread.detach();
}


ResourceManager::~ResourceManager()
{

}

void ResourceManager::AddToWaitingList(Resource* res) {
	std::lock_guard<std::mutex> lock(mylock);
	waitingList.push_back(res);
	loadCondition.notify_one();
}

void ResourceManager::LoadDrawable(RpfResourceFileEntry* entry, Resource* res) {
	res->Buffer.resize(entry->SystemSize + entry->GraphicsSize);
	gameworld->getGameData()->ExtractFileResource(*(entry), res->Buffer);
	res->SystemSize = entry->SystemSize;

	auto it = gameworld->getGameData()->YtdEntries.find(res->TextureDictionaryHash);
	if (it == gameworld->getGameData()->YtdEntries.end())
	{
		res->TextureDictionaryHash = 0;
	}

	gameworld->resources_lock.lock();
	gameworld->resources.push_back(res);
	gameworld->resources_lock.unlock();
}

void ResourceManager::update()
{
	while (true) {
		std::unique_lock<std::mutex> lock(mylock);

		loadCondition.wait(lock, [this] {
			return !waitingList.empty();
		});

		auto res = waitingList.back();
		waitingList.pop_back();
		lock.unlock();

		switch (res->type) {
			case ymap: {
				auto it = gameworld->getGameData()->YmapEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YmapEntries.end())
				{
					res->Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);

					YmapLoader* iter = static_cast<YmapLoader*>(res->file);

					memstream2 stream(res->Buffer.data(), res->Buffer.size());
					iter->Init(stream);

					for (auto& object : *iter->Objects)
					{
						std::unordered_map<uint32_t, CBaseArchetypeDef>::iterator it = gameworld->getGameData()->CBaseArchetypeDefs.find(object.CEntity.archetypeName);
						if (it != gameworld->getGameData()->CBaseArchetypeDefs.end())
						{
							object.Archetype._BaseArchetypeDef = it->second;

							object.BoundPos = object.CEntity.position - object.Archetype._BaseArchetypeDef.bsCentre;
							object.BoundRadius = object.Archetype._BaseArchetypeDef.bsRadius * std::max(object.CEntity.scaleXY, object.CEntity.scaleZ);

							if (object.CEntity.lodDist <= 0) object.CEntity.lodDist = it->second.lodDist;
							if (object.CEntity.childLodDist <= 0) object.CEntity.childLodDist = it->second.lodDist;

							object.type = 1;
						}
						else {
							std::unordered_map<uint32_t, CTimeArchetypeDef>::iterator it = gameworld->getGameData()->CTimeArchetypeDefs.find(object.CEntity.archetypeName);
							if (it != gameworld->getGameData()->CTimeArchetypeDefs.end())
							{
								object.Archetype = it->second;

								object.BoundPos = object.CEntity.position; //object.BoundPos = object.Archetype._BaseArchetypeDef.bsCentre + object.CEntity.position;
								object.BoundRadius = object.Archetype._BaseArchetypeDef.bsRadius * std::max(object.CEntity.scaleXY, object.CEntity.scaleZ);

								if (object.CEntity.lodDist <= 0) object.CEntity.lodDist = it->second._BaseArchetypeDef.lodDist;
								if (object.CEntity.childLodDist <= 0) object.CEntity.childLodDist = it->second._BaseArchetypeDef.lodDist;

								object.type = 2;
							}
						}

						object.CEntity.lodDist *= object.CEntity.lodDist;
						object.CEntity.childLodDist *= object.CEntity.childLodDist;

					}

					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
				else {
					delete res;
				}
			}
					   break;
			case ydr:
			{
				auto it = gameworld->getGameData()->YdrEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YdrEntries.end())
				{
					LoadDrawable(it->second, res);
				}
				else {
					delete res; //RARELY HAPPENS
				}
			}
			break;
			case ydd:
			{
				auto it = gameworld->getGameData()->YddEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YddEntries.end())
				{
					LoadDrawable(it->second, res);
				}
				else {
					delete res;
				}
			}
			break;
			case yft:
			{
				auto it = gameworld->getGameData()->YftEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YftEntries.end())
				{
					LoadDrawable(it->second, res);
				}
				else {
					delete res;
				}
			}
			break;
			case ytd:
			{
				auto it = gameworld->getGameData()->YtdEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YtdEntries.end())
				{
					res->Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);
					res->SystemSize = it->second->SystemSize;
					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
				else {
					delete res;
				}
			}

			break;
			case ybn:
			{
				auto it = gameworld->getGameData()->YbnEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YbnEntries.end())
				{
					res->Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);
					res->SystemSize = it->second->SystemSize;

					memstream2 stream(res->Buffer.data(), res->Buffer.size());
					res->file->Init(stream);

					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
				else {
					delete res;
				}
			}
			break;
		}

	}
}
