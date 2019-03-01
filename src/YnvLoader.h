#pragma once
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "YdrLoader.h"
#include <Recast.h>

class YnvLoader
{
public:
    YnvLoader(memstream& file);
    ~YnvLoader();
};
