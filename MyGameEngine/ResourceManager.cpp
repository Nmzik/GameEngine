#include "ResourceManager.h"

ResourceManager::ResourceManager(GameWorld *world) : gameworld{ world }
{
	//printf("RESOURCE MANAGER LOADED!\n");
	ResourcesThread = std::thread(&ResourceManager::update, this);
}


ResourceManager::~ResourceManager()
{

}

void ResourceManager::update()
{
	while (true) {
		std::unique_lock<std::mutex> lock(mylock);

		if (waitingList.size() > 0) {
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
					res->SystemSize = it->second->SystemSize;

					auto iter = gameworld->ymapLoader.find(res->Hash);
					if (iter != gameworld->ymapLoader.end())
					{
						memstream2 stream(res->Buffer.data(), res->Buffer.size());
						iter->second->Init(stream);

						for (auto& object : iter->second->Objects)
						{
							std::unordered_map<uint32_t, CBaseArchetypeDef>::iterator it = gameworld->getGameData()->CBaseArchetypeDefs.find(object.CEntity.archetypeName);
							if (it != gameworld->getGameData()->CBaseArchetypeDefs.end())
							{
								object.Archetype._BaseArchetypeDef = it->second;

								object.BoundPos = object.CEntity.position; //object.BoundPos = object.Archetype._BaseArchetypeDef.bsCentre + object.CEntity.position;
								object.BoundRadius = object.Archetype._BaseArchetypeDef.bsRadius * std::max(object.CEntity.scaleXY, object.CEntity.scaleZ);
								if (object.CEntity.lodDist <= 0) object.CEntity.lodDist = it->second.lodDist;
								if (object.CEntity.childLodDist <= 0) object.CEntity.childLodDist = it->second.lodDist;

								object.type = 1;

								//object.FoundArchetype = true;
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

									//object.FoundArchetype = true;
								}
							}

							object.CEntity.lodDist *= object.CEntity.lodDist;
							object.CEntity.childLodDist *= object.CEntity.childLodDist;

						}

					}

					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
			}
			break;
			case ydr:
			{
				auto it = gameworld->getGameData()->YdrEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YdrEntries.end())
				{
					res->Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);
					res->SystemSize = it->second->SystemSize;
					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
				else {
					printf("");
				}
			}
			break;
			case ydd:
			{
				auto it = gameworld->getGameData()->YddEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YddEntries.end())
				{
					res->Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);
					res->SystemSize = it->second->SystemSize;
					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
			}
			break;
			case yft:
			{
				auto it = gameworld->getGameData()->YftEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YftEntries.end())
				{
					res->Buffer.resize(it->second->SystemSize + it->second->GraphicsSize);
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);
					res->SystemSize = it->second->SystemSize;
					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
				else {
					printf("");
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
					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
			}
			break;
			}

		}
	}
}
