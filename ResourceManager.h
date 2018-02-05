#pragma once

#include <thread>
#include <mutex>
#include "Model.h"
#include "GameWorld.h"

class GameWorld;

class ResourceManager
{
	std::thread LoadingResourcesThread;
	std::mutex mylock;

	std::vector <Model*> waitingList;
	GameWorld *gameworld;
public:
	std::mutex mainLock;

	ResourceManager(GameWorld *world);
	~ResourceManager();

	void AddToWaitingList(Model* model) {
		mylock.lock();
		waitingList.push_back(model);
		mylock.unlock();
	}

	void update();
};

