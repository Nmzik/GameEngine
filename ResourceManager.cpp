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
		std::vector<Model>::iterator i = waitingList.begin();
		while (i != waitingList.end())
		{
			i->Load();
			gameworld->models.push_back(*i);
			i = waitingList.erase(i);
		}
		mylock.unlock();
	}
}
