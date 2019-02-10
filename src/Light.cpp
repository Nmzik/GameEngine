#include "Light.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool castsShadows)
    : direction(direction)
    , ambient(ambient)
    , diffuse(diffuse)
    , specular(specular)
    , castsShadows(castsShadows)
{
}
