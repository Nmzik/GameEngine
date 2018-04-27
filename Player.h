#pragma once

#include "Vehicle.h"
#include "YddLoader.h"
#include <array>

struct CharacterWeaponSlot {
	// Assuming these match the entries in weapon.dat
	uint32_t weaponId;
	uint32_t bulletsClip;
	uint32_t bulletsTotal;
};

class Player
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
	bool on_ground = false;
	bool isAlive() {
		return health > 0;
	}

	void TakeDamage(float dmg);
	glm::mat4& getPosition();
	void PhysicsTick();
	void ExitVehicle();
	void EnterVehicle(Vehicle* nearestVehicle);
	Vehicle* GetCurrentVehicle();
	void addToInventory(uint32_t slot, uint32_t ammo);
	void setActiveWeapon(uint32_t slot);
	void Jump();
	btRigidBody* getPhysCharacter();
	void Draw(Shader* shader);
};

