#pragma once
#include "YdrLoader.h"

class YddLoader
{
public:
	YddLoader * next;
	std::unordered_map<uint32_t, YdrLoader*> YdrFiles;
	YtdLoader * externalYtd = nullptr;
	uint32_t RefCount = 0;
	bool Loaded = false;

	void Init(memstream2 & file, int32_t systemSize, btDiscreteDynamicsWorld * world);
	void Remove();
};

class YddPool
{
public:
	YddPool();
	~YddPool();

	YddLoader* Load();
	void Remove(YddLoader* ymap);

	YddLoader ydds[1000];

private:
	YddLoader * firstAvailable_;
};