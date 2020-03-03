#pragma once
#include <vector>

#include "FileType.h"

class AwcLoader
{
public:
    AwcLoader(memstream& file);
    ~AwcLoader();
};
