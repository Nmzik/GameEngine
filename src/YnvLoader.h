#pragma once
#include "YdrLoader.h"
#include <Recast.h>
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"

class YnvLoader
{
public:
	YnvLoader(memstream& file);
	~YnvLoader();
};
