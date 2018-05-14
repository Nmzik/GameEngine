#pragma once
#include "YdrLoader.h"

class YftLoader
{
public:
	YdrLoader * YdrFile;
	uint32_t time;
	std::vector<YdrLoader*> wheels;
	bool Loaded = false;

	YftLoader();
	void Init(memstream& file, int32_t systemSize, bool need, btDiscreteDynamicsWorld* world);
	YftLoader(memstream& file, int32_t systemSize, bool need, btDiscreteDynamicsWorld* world);
	~YftLoader();
};

