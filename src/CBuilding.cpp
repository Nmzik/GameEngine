#include "CBuilding.h"

#include <btBulletDynamicsCommon.h>

#include "loaders/YddLoader.h"
#include "loaders/YdrLoader.h"
#include "loaders/YftLoader.h"
#include "loaders/YtdLoader.h"

CBuilding::CBuilding(fwEntityDef def)
    : CEntity(def.position, glm::quat(def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ), ObjectType::Building)
    , entityDef(def)
{
}

CBuilding::~CBuilding()
{
}
