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
	std::vector<YdrLoader*> playerModel;
	glm::mat4 model;
public:
	Player();
	~Player();
	Player* next;
	bool on_ground = false;
	bool isAlive() {
		return health > 0;
	}

	bool loaded = false;

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
	void Init(glm::vec3 position, YddLoader* ydd, btDiscreteDynamicsWorld* world);
	void Remove();
};

class PedPool {
public:
	PedPool();

	Player peds[20];
	Player * firstAvailable_;

	void Add(glm::vec3 position, YddLoader * ydd, btDiscreteDynamicsWorld * world) {
		// Make sure the pool isn't full.
		assert(firstAvailable_ != NULL);

		// Remove it from the available list.
		Player* newPlayer = firstAvailable_;
		firstAvailable_ = newPlayer->next;

		newPlayer->Init(position, ydd, world);
	}

	void Remove(Player* player) {
		player->next = firstAvailable_;
		firstAvailable_ = player;
		player->Remove();
	}
};
