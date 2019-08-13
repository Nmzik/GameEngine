#pragma once

#include <vector>

#ifdef WIN32
#include "opengl/Geometry.h"
#else
#include "metal/Geometry.h"
#endif

class Model
{
public:
    Model() = default;
    ~Model() = default;

    uint32_t Unk_2Ch;
    std::vector<Geometry> geometries;
};
