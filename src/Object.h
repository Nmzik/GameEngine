#pragma once
#include "Entity.h"
#include "YtypLoader.h"

class YdrLoader;
class YddLoader;
class YftLoader;
class YtdLoader;
class btRigidBody;

class Object : public Entity
{
	public:
	YdrLoader* ydr         = nullptr;
	YddLoader* ydd         = nullptr;
	YftLoader* yft         = nullptr;
	YtdLoader* ytd         = nullptr;
	btRigidBody* rigidBody = nullptr;
	bool Loaded            = false;
	//	uint8_t type = 0;

	glm::vec3 BoundPos;
	float BoundRadius;
	fwEntityDef CEntity;
	Archetype* archetype;

	Object(fwEntityDef def);
	~Object();

	private:
};
