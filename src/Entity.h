#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class Entity
{
    bool visible;

protected:
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    float health = 100.0f;

public:
    Entity(glm::vec3& pos, glm::quat rot, glm::vec3 scale_);
    ~Entity();

    glm::vec3 getPosition() const
    {
        return position;
    }

    glm::mat4 getMatrix() const
    {
        return modelMatrix;
    }
};