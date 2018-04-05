#pragma once

#include "Vehicle.h"
#include "YddLoader.h"
//#include "GameWorld.h"

//class GameWorld;

class Player
{
	btKinematicCharacterController *physCharacter;
	btCapsuleShapeZ* physShape;
	float health = 100.0f;
	bool inWater = false;
	glm::vec3 playerDirection;
	Vehicle* vehicle;
	YddLoader* player;
public:
	bool Enabled = true;
	Player(glm::vec3 position, YddLoader* ydd, btDiscreteDynamicsWorld* world);
	~Player();

	bool isAlive() {
		return health > 0;
	}

	void TakeDamage(float dmg);
	glm::mat4 getPosition();
	void PhysicsTick();
	void ExitVehicle();
	void EnterVehicle(Vehicle* nearestVehicle);
	Vehicle* GetCurrentVehicle();
	void Jump();
	btKinematicCharacterController* getPhysCharacter();
	void Draw();
};

