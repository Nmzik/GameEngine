#pragma once
#include "FileType.h"

#include "YdrLoader.h"
#include "YddLoader.h"
#include "YftLoader.h"
#include "YtdLoader.h"
#include "YbnLoader.h"
#include "YmapLoader.h"

class GlobalPool
{
public:
	static GlobalPool& getInstance()
	{
		return pool;
	}
	static GlobalPool pool;

	Pool<YdrLoader, 6000> YdrPool;
	Pool<YbnLoader, 1000> YbnPool;
	Pool<YddLoader, 1000> YddPool;
	Pool<YftLoader, 1000> YftPool;
	Pool<YmapLoader, 1000> YmapPool;
	Pool<YtdLoader, 2500> YtdPool;
};