#pragma once
#include "YdrLoader.h"

class YftLoader : public FileType
{
public:
	YftLoader * next;
	YdrLoader * YdrFile = nullptr;
	std::vector<YdrLoader*> wheels;

	void Init(memstream2& file, int32_t systemSize) override;
	void Remove();
};

class YftPool
{
public:
	YftPool();
	~YftPool();

	YftLoader* Load();
	void Remove(YftLoader* yft);

	YftLoader yfts[1000];

private:
	YftLoader * firstAvailable_;
};
