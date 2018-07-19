#pragma once

#include "Entity.h"
#include "Vehicle.h"
#include "YddLoader.h"
#include <array>

struct CharacterWeaponSlot {
	// Assuming these match the entries in weapon.dat
	uint32_t weaponId;
	uint32_t bulletsClip;
	uint32_t bulletsTotal;
};

class Player : public Entity
{
	btCapsuleShapeZ* physShape;
	btRigidBody *body;

	std::array<CharacterWeaponSlot, 13> weapons;
	uint16_t currentWeapon = 0;

	float health = 100.0f;
	bool inWater = false;
	glm::vec3 playerDirection;
	btDiscreteDynamicsWorld* World;
	Vehicle* vehicle;
	YddLoader* player;
	glm::mat4 model;
public:
	Player(glm::vec3 position, YddLoader* ydd, btDiscreteDynamicsWorld* world);
	~Player();

	std::vector<YdrLoader*> playerModel;
	Player* next;
	bool on_ground = false;
	bool isAlive() {
		return health > 0;
	}

	float getCurrentHealth() {
		return health;
	}

	bool loaded = false;

	void SetPosition(glm::vec3& pos);

	void TakeDamage(float dmg);
	glm::vec3& getPos();
	glm::mat4& getPosition();
	void PhysicsTick();
	void ExitVehicle();
	void EnterVehicle(Vehicle* nearestVehicle);
	Vehicle* GetCurrentVehicle();
	void addToInventory(uint32_t slot, uint32_t ammo);
	void setActiveWeapon(uint32_t slot);
	void Jump();
	btRigidBody* getPhysCharacter();
};
