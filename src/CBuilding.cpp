#include "CBuilding.h"
#include "YddLoader.h"
#include "YdrLoader.h"
#include "YftLoader.h"
#include "YtdLoader.h"

#include <btBulletDynamicsCommon.h>

CBuilding::CBuilding(fwEntityDef def)
    : CEntity(def.position, def.rotation, glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ), ObjectType::Building)
    , EntityDef(def)
{
}

CBuilding::~CBuilding()
{
    /*if (rigidBody)
	{
	 delete rigidBody->getMotionState();
	 delete rigidBody;
	}*/
    if (archetype)
    {
        switch (archetype->BaseArchetypeDef.assetType)
        {
            case ASSET_TYPE_DRAWABLE:
                if (ydr)
                    ydr->RefCount--;
                break;
            case ASSET_TYPE_DRAWABLEDICTIONARY:
                if (ydd)
                    ydd->RefCount--;
                break;
            case ASSET_TYPE_FRAGMENT:
                if (yft)
                    yft->RefCount--;
                break;
        }
    }
    if (ytd)
        ytd->RefCount--;
}
