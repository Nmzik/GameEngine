#pragma once
#include "YdrLoader.h"

class YddLoader
{
public:
	std::unordered_map<uint32_t, YdrLoader*> YdrFiles;
	YtdLoader * externalYtd = nullptr;
	uint32_t time;
	bool Loaded = false;

	YddLoader();
	void Init(memstream& file, int32_t systemSize, btDiscreteDynamicsWorld* world);
	YddLoader(memstream& file, int32_t systemSize, btDiscreteDynamicsWorld* world);
	~YddLoader();
};

