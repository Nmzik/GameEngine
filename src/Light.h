#pragma once
#include "glm/glm.hpp"

class PointLight
{
public:
    PointLight()
    {
    }
    ~PointLight()
    {
    }
};

class DirectionalLight
{
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    bool castsShadows;

    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool castsShadows = false);
    ~DirectionalLight()
    {
    }
};
