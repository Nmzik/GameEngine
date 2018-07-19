#pragma once

#include <thread>
#include <mutex>
#include "includes.h"
#include "RpfEntry.h"

class GameWorld;

class ResourceManager
{
	std::thread ResourcesThread;
	std::mutex mylock;

	std::vector<Resource*> waitingList;
	GameWorld *gameworld;
public:

	ResourceManager(GameWorld *world);
	~ResourceManager();

	void LoadDrawable(RpfResourceFileEntry * entry, Resource * res);

	void AddToWaitingList(Resource* res) {
		std::lock_guard<std::mutex> lock(mylock);

		waitingList.push_back(res);
	}

	void update();
};

