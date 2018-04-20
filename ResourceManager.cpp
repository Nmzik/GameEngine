#include "ResourceManager.h"

ResourceManager::ResourceManager(GameWorld *world) : gameworld{ world }
{
	printf("RESOURCE MANAGER LOADED!\n");
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
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);

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
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);

					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
			}
				break;
			case ydd:
			{
				auto it = gameworld->getGameData()->YddEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YddEntries.end())
				{
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);

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
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);

					gameworld->resources_lock.lock();
					gameworld->resources.push_back(res);
					gameworld->resources_lock.unlock();
				}
			}
				break;
			case ytd:
			{
				auto it = gameworld->getGameData()->YtdEntries.find(res->Hash);
				if (it != gameworld->getGameData()->YtdEntries.end())
				{
					gameworld->getGameData()->ExtractFileResource(*(it->second), res->Buffer);

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
