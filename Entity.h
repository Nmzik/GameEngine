#pragma once
#include "glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "gtc/matrix_transform.hpp"

class Entity
{
	//glm::mat4 modelMatrix;
	//YdrLoader maybe?

	bool visible;
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Entity(glm::vec3& pos, glm::quat rot, glm::vec3 Scale)
		:
		position(pos),
		rotation(rot),
		scale(Scale),
		visible(true)
		//modelMatrix(glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(glm::quat(-rotation.w, rotation.x, rotation.y, rotation.z)) * glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z)))
	{

	}

	~Entity() {

	}

private:

};