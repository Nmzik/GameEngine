#pragma once
#include "CEntity.h"

#include "loaders/YtypLoader.h"

class YdrLoader;
class YddLoader;
class YftLoader;
class YtdLoader;
class btRigidBody;

class CBuilding : public CEntity
{
public:
    YdrLoader* ydr = nullptr;
    YddLoader* ydd = nullptr;
    YftLoader* yft = nullptr;
    YtdLoader* ytd = nullptr;
    btRigidBody* rigidBody = nullptr;
    bool loaded = false;
    //	uint8_t type = 0;

    glm::vec3 boundPos;
    float boundRadius;
    fwEntityDef entityDef;
    fwArchetype* archetype = nullptr;

    CBuilding(fwEntityDef& def);
    ~CBuilding();

private:
};
