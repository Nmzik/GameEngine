#include "CBuilding.h"
#include "loaders/YddLoader.h"
#include "loaders/YdrLoader.h"
#include "loaders/YftLoader.h"
#include "loaders/YtdLoader.h"

#include <btBulletDynamicsCommon.h>

CBuilding::CBuilding(fwEntityDef def)
    : CEntity(def.position, def.rotation, glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ), ObjectType::Building)
    , entityDef(def)
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
                    ydr->refCount--;
                break;
            case ASSET_TYPE_DRAWABLEDICTIONARY:
                if (ydd)
                    ydd->refCount--;
                break;
            case ASSET_TYPE_FRAGMENT:
                if (yft)
                    yft->refCount--;
                break;
            case ASSET_TYPE_UNINITIALIZED:
            case ASSET_TYPE_ASSETLESS:
                break;
        }
    }
    if (ytd)
        ytd->refCount--;
}
