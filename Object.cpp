#include "Object.h"

Object::Object(CEntityDef def) : Entity(def), Drawable(nullptr), ModelMatrix(glm::translate(glm::mat4(), def.position) * glm::toMat4(glm::quat(-def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z)) * glm::scale(glm::mat4(), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ)))
{

}

Object::~Object()
{
}