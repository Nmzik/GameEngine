#pragma once
#include "YdrLoader.h"
#include "YtypLoader.h"

class Object
{
	glm::mat4 ModelMatrix;
public:
	bool loaded = false;
	bool ArchetypeFound = false;
	uint32_t time;
	YdrLoader* Drawable;
	CEntityDef Entity;
	CTimeArchetypeDef Archetype;

	Object(CEntityDef def);
	~Object();

	glm::mat4& getMatrix() {
		return ModelMatrix;
	}

private:

};