#pragma once

#include <btBulletDynamicsCommon.h>
#include "Entity.h"
#include <array>
#include <vector>

class CVehicle;
class YddLoader;
class YdrLoader;

struct CharacterWeaponSlot
{
	// Assuming these match the entries in weapon.dat
	uint32_t weaponId;
	uint32_t bulletsClip;
	uint32_t bulletsTotal;
};

class CPed : public Entity
{
	std::unique_ptr<btCapsuleShapeZ> physShape;
	std::unique_ptr<btRigidBody> body;
	std::unique_ptr<btMotionState> myMotionState;

	std::array<CharacterWeaponSlot, 13> weapons;
	uint16_t currentWeapon = 0;
	bool inWater = false;
	glm::vec3 playerDirection;
	CVehicle* vehicle;
	YddLoader* player;

public:
	CPed(glm::vec3 position, YddLoader* ydd);
	~CPed();

	CPed& operator=(CPed&& other) = default;
	CPed(CPed&& other) = default;

	std::vector<YdrLoader*> playerModel;
	bool on_ground = false;
	bool isAlive()
	{
		return health > 0;
	}

	float getCurrentHealth()
	{
		return health;
	}

	void SetPosition(glm::vec3 pos);
	glm::vec3& getPosition();
	glm::mat4& getMatrix();

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
