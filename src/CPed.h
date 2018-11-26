#pragma once

#include "Entity.h"
#include "CVehicle.h"
#include "YddLoader.h"
#include <array>

struct CharacterWeaponSlot
{
	// Assuming these match the entries in weapon.dat
	uint32_t weaponId;
	uint32_t bulletsClip;
	uint32_t bulletsTotal;
};

class CPed : public Entity
{
	btCapsuleShapeZ* physShape;
	btRigidBody* body;

	atArray<CharacterWeaponSlot> weapons;
	uint16_t currentWeapon = 0;
	bool inWater = false;
	glm::vec3 playerDirection;
	CVehicle* vehicle;
	YddLoader* player;

public:
	CPed(glm::vec3 position, YddLoader* ydd, btDiscreteDynamicsWorld* world);
	~CPed();

	std::vector<YdrLoader*> playerModel;
	CPed* next;
	bool on_ground = false;
	bool isAlive()
	{
		return health > 0;
	}

	float getCurrentHealth()
	{
		return health;
	}

	void SetPosition(glm::vec3& pos);
	glm::vec3& getPos();
	glm::mat4& getPosition();

	void TakeDamage(float dmg);
	void PhysicsTick();
	void ExitVehicle();
	void EnterVehicle(CVehicle* nearestVehicle);
	CVehicle* GetCurrentVehicle();
	void addToInventory(uint32_t slot, uint32_t ammo);
	void setActiveWeapon(uint32_t slot);
	void Jump();
	btRigidBody* getPhysCharacter();
};
