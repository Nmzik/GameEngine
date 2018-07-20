#pragma once
#include "YdrLoader.h"
#include <chrono>

class YddLoader : public FileType
{
public:
	YddLoader * next;
	std::unordered_map<uint32_t, YdrLoader*> YdrFiles;
	uint32_t RefCount = 0;
	bool Loaded = false;

	void Init(memstream2 & file, int32_t systemSize) override;
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
