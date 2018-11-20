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
	if (rigidBody) {
		delete rigidBody->getMotionState();
		delete rigidBody;
		rigidBody = nullptr;
	}

	if (ydd)
		ydd->RefCount--;
	else if (yft)
		yft->RefCount--;
	else if (ydr)
		ydr->RefCount--;
	if (ytd)
		ytd->RefCount--;
}
