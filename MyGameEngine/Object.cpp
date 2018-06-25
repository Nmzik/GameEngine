#include "Object.h"
#include "YddLoader.h"
#include "YftLoader.h"

Object::Object(CEntityDef def)
	: Entity(def.position, glm::quat(-def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ))
	, CEntity(def)
	, ModelMatrix(glm::translate(glm::mat4(1.0f), def.position) * glm::mat4_cast(glm::quat(-def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z)) * glm::scale(glm::mat4(1.0f), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ)))
{

}

Object::~Object()
{
	if (ydr)
		ydr->RefCount--;
	if (ydd)
		ydd->RefCount--;
	if (yft)
		yft->RefCount--;
	if (ytd)
		ytd->RefCount--;
}
