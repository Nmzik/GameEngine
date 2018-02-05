#pragma once
#include "fstream"
#include <iostream>
#include "zlib.h"
#include "RpfEntry.h"

class YmapLoader
{
public:
	YmapLoader(memstream& file);
	~YmapLoader();
};

