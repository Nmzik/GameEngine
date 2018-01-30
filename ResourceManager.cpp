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
		if (waitingList.size() > 0) {
			auto& model = waitingList.back();
			model.Load();
			gameworld->models.emplace_back(std::move(model));
			waitingList.pop_back();
		}
		mylock.unlock();
	}
}
