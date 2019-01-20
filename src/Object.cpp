#include "Object.h"
#include "YdrLoader.h"
#include "YddLoader.h"
#include "YftLoader.h"
#include "YtdLoader.h"

#include <btBulletDynamicsCommon.h>

Object::Object(fwEntityDef def)
	: Entity(def.position, def.rotation, glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ))
	, CEntity(def)
{
}

Object::~Object()
{
	/*if (rigidBody)
	{
		delete rigidBody->getMotionState();
		delete rigidBody;
	}*/
	if (archetype) {
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
