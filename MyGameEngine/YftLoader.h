#pragma once
#include "YdrLoader.h"

class YftLoader
{
public:
	YdrLoader * YdrFile = nullptr;
	uint32_t RefCount = 0;
	std::vector<YdrLoader*> wheels;
	bool Loaded = false;

	void Init(memstream2& file, int32_t systemSize, bool need, btDiscreteDynamicsWorld* world);
	~YftLoader();
};

