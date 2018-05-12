#pragma once
#include "YdrLoader.h"

class YddLoader
{
public:
	std::unordered_map<uint32_t, YdrLoader*> YdrFiles;
	YtdLoader * externalYtd;
	uint32_t time;
	YddLoader(memstream& file, int32_t systemSize, btDiscreteDynamicsWorld* world);
	~YddLoader();
};

