#include "ResourceManager.h"

ResourceManager::ResourceManager(GameWorld *world) : gameworld{world}
{
	printf("RESOURCE MANAGER LOADED!\n");

	LoadingResourcesThread = std::thread(&ResourceManager::update, this);
}


ResourceManager::~ResourceManager()
{

}

void ResourceManager::update()
{
	while (true) {
		mylock.lock();

		for (int i = 0; i < waitingList.size(); i++) {
			waitingList[i].Load();
			gameworld->models.emplace_back(std::move(waitingList[i]));
			waitingList.erase(waitingList.begin() + i);
		}
		mylock.unlock();

		/*or (int i = 0; i < gameworld->models.size; i++)
		{
			if (gameworld->models[i].BuffersCreated) 
		}*/
	}
}
