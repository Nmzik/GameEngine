#pragma once
#include "FileType.h"

class YdrLoader;
class YbnLoader;
class YddLoader;
class YftLoader;
class YmapLoader;
class YtdLoader;

class GlobalPool
{
public:
    static GlobalPool& getInstance()
    {
        return pool;
    }
    static GlobalPool pool;

    //	Pool<YdrLoader, 6000> YdrPool;
    //	Pool<YbnLoader, 1000> YbnPool;
    //	Pool<YddLoader, 1000> YddPool;
    //	Pool<YftLoader, 1000> YftPool;
    //	Pool<YmapLoader, 1000> YmapPool;
    //	Pool<YtdLoader, 2500> YtdPool;
};