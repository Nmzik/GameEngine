#pragma once

#include <vector>

#include "Geometry.h"

class Model
{
public:
    Model() = default;
    ~Model() = default;

    uint32_t Unk_2Ch;
    std::vector<Geometry> geometries;
};
