#include "Entity.h"

Entity::Entity(glm::vec3& pos, glm::quat rot, glm::vec3 scale_)
    : position(pos)
    , rotation(rot)
    , scale(scale_)
    , visible(true)
{
    modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

Entity::~Entity()
{
}