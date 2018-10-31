#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Entity
{
	bool visible;
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 modelMatrix;

	float health = 100.0f;

	Entity(glm::vec3& pos, glm::quat rot, glm::vec3 scale_)
		:
		position(pos),
		rotation(rot),
		scale(scale_),
		visible(true)
	{
		modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
	}

	~Entity() {

	}

	glm::mat4& getMatrix() {
		return modelMatrix;
	}

private:

};