#include "CEntity.h"

CEntity::CEntity(glm::vec3& pos, glm::quat rot, glm::vec3 scale_, ObjectType _type)
    : visible(true)
    , type(_type)
    , position(pos)
    , rotation(rot)
    , scale(scale_)
{
    modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

CEntity::~CEntity()
{
}
