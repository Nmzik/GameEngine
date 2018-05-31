#include "Object.h"

Object::Object(CEntityDef def)
	: Entity(def.position, glm::quat(-def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ))
	, CEntity(def)
	, ModelMatrix(glm::translate(glm::mat4(1.0f), def.position) * glm::mat4_cast(glm::quat(-def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z)) * glm::scale(glm::mat4(1.0f), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ)))
{

}

Object::~Object()
{
	if (Loaded) {
		switch (Archetype._BaseArchetypeDef.assetType)
		{
			case ASSET_TYPE_DRAWABLE:
				ydr->RefCount--;
				break;
			case ASSET_TYPE_DRAWABLEDICTIONARY:
				ydd->RefCount--;
				break;
			case ASSET_TYPE_FRAGMENT:
				yft->RefCount--;
				break;
		}
	}
}