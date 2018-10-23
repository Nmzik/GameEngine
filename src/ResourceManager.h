#pragma once

#include <thread>
#include <mutex>
#include "includes.h"

class GameWorld;
class RpfEntry;

class RpfResourceFileEntry;

class ResourceManager
{
	std::thread ResourcesThread;
	std::mutex mylock;
	std::condition_variable loadCondition;
	bool running;

	std::vector<Resource> waitingList;
	GameWorld *gameworld;
public:

	ResourceManager(GameWorld *world);
	~ResourceManager();

	void LoadDrawable(RpfResourceFileEntry * entry, Resource& res);
	inline void AddToMainQueue(Resource& res);
	void AddToWaitingList(Resource& res);

	void update();
};

