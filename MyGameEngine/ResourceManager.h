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
	std::condition_variable loadCondition;

	std::vector<Resource*> waitingList;
	GameWorld *gameworld;
public:

	ResourceManager(GameWorld *world);
	~ResourceManager();

	void LoadDrawable(RpfResourceFileEntry * entry, Resource * res);

	void AddToWaitingList(Resource* res);

	void update();
};

