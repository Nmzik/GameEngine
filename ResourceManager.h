#pragma once

#include <thread>
#include <mutex>
#include "includes.h"
#include "GameWorld.h"

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

	void AddToWaitingList(Resource* res) {
		mylock.lock();

		bool found = false;

		for (int i = 0; i < waitingList.size(); i++)
		{
			if (waitingList[i]->Hash == res->Hash && waitingList[i]->type == res->type) found = true;
		}

		if (!found)
			waitingList.push_back(res);

		mylock.unlock();
	}

	void update();
};

