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
			//if (i->dynamic) gameworld->GetDynamicsWorld()->addRigidBody(gameworld->models.back().getBody());
			i = waitingList.erase(i);
		}
		mylock.unlock();
	}
}
