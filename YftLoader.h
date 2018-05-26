#pragma once
#include "YdrLoader.h"

class YftLoader
{
public:
	YdrLoader * YdrFile = nullptr;
	uint32_t time;
	std::vector<YdrLoader*> wheels;
	bool Loaded = false;

	void Init(memstream& file, int32_t systemSize, bool need, btDiscreteDynamicsWorld* world);
	~YftLoader();
};

