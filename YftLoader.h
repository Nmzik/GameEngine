#pragma once
#include "YdrLoader.h"

class YftLoader
{
public:
	YdrLoader * YdrFile;
	uint32_t time;
	std::vector<YdrLoader*> wheels;

	YftLoader(memstream& file, bool need, btDiscreteDynamicsWorld* world);
	~YftLoader();
};

