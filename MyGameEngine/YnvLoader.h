#pragma once
#include "YdrLoader.h"
#include "DetourNavMeshBuilder.h"

class YnvLoader
{
public:
	YnvLoader(memstream& file);
	~YnvLoader();
};

