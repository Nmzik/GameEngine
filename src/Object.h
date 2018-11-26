#pragma once
#include "YtypLoader.h"
#include "Entity.h"

class YdrLoader;
class YddLoader;
class YftLoader;
class YtdLoader;
class btRigidBody;

class Object : public Entity
{
public:
	YdrLoader* ydr = nullptr;
	YddLoader* ydd = nullptr;
	YftLoader* yft = nullptr;
	YtdLoader* ytd = nullptr;
	btRigidBody* rigidBody = nullptr;
	bool Loaded = false;
	bool FoundModel = false;
	//	uint8_t type = 0;

	glm::vec3 BoundPos;
	float BoundRadius;
	fwEntityDef CEntity;
	Archetype* Archetype;

	Object(fwEntityDef def);
	~Object();

private:
};
