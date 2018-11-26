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

class DirectonalLight
{
public:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	bool castsShadows;

	DirectonalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool castsShadows = false);
	~DirectonalLight()
	{
	}
};
