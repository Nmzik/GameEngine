#pragma once
#include "YdrLoader.h"
#include "YtypLoader.h"
#include "Entity.h"

class Object : public Entity
{
	glm::mat4 ModelMatrix;
public:
	//bool loaded = false;
	uint8_t type = 0;
	CEntityDef CEntity;
	CTimeArchetypeDef Archetype;

	Object(CEntityDef def);
	~Object();

	glm::mat4& getMatrix() {
		return ModelMatrix;
	}

private:

};