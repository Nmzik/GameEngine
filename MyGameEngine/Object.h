#pragma once
#include "YtypLoader.h"
#include "Entity.h"

class YdrLoader;
class YddLoader;
class YftLoader;

class Object : public Entity
{
	glm::mat4 ModelMatrix;
public:
	YdrLoader* ydr = nullptr;
	YddLoader* ydd = nullptr;
	YftLoader* yft = nullptr;
	bool Loaded = false;
	bool FoundArchetype = false;
	bool FoundModel = false;
	bool FoundBaseModel = false;
	uint8_t type = 0;

	glm::vec3 BoundPos;
	float BoundRadius;
	CEntityDef CEntity;
	CTimeArchetypeDef Archetype;

	Object(CEntityDef def);
	~Object();

	glm::mat4& getMatrix() {
		return ModelMatrix;
	}

private:

};