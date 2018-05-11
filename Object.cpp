#include "Object.h"

Object::Object(CEntityDef def) : Entity(def), Drawable(nullptr), ModelMatrix(glm::translate(glm::mat4(1.0f), def.position) * glm::mat4_cast(glm::quat(-def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z)) * glm::scale(glm::mat4(1.0f), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ)))
{

}

Object::~Object()
{
}