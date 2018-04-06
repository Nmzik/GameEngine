#include "Player.h"

Player::Player(glm::vec3 position, YddLoader* ydd, btDiscreteDynamicsWorld* world) : player(ydd)
{
	btPairCachingGhostObject* physObject = new btPairCachingGhostObject();
	physShape = new btCapsuleShapeZ(0.45f, 1.2f);
	physObject->setUserPointer(this);
	physObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
	physObject->setCollisionShape(physShape);
	physObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	physCharacter = new btKinematicCharacterController(physObject, physShape, 0.30f);
	physCharacter->setFallSpeed(100.f);
	physCharacter->setUseGhostSweepTest(false);
	//physCharacter->setVelocityForTimeInterval(btVector3(1.f, 1.f, 1.f), 1.f);
	physCharacter->setGravity(world->getGravity());
	world->addCollisionObject(physObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	world->addAction(physCharacter);	

	playerDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

Player::~Player()
{
	delete player;
	//printf("DESTRUCTOR \n");
}

btKinematicCharacterController* Player::getPhysCharacter()
{
	return physCharacter;
}

void Player::TakeDamage(float dmg)
{
	health -= dmg;
}

glm::mat4 Player::getPosition()
{
	if (physCharacter->getGhostObject()->getWorldTransform().getOrigin().getZ() <= -150) {
		printf("MAYBE A BUG!\n");
		physCharacter->getGhostObject()->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), physCharacter->getGhostObject()->getWorldTransform().getOrigin() + btVector3(0, 0, 300)));
	}

	glm::mat4 model;
	physCharacter->getGhostObject()->getWorldTransform().getOpenGLMatrix(&model[0][0]);

	return model;
}

void Player::PhysicsTick()
{
	physCharacter->setWalkDirection(btVector3(playerDirection.x, playerDirection.y, playerDirection.z));
}

void Player::ExitVehicle()
{
	vehicle->SetThrottle(1.0f);
	vehicle->SetSteeringValue(0.0f);
	vehicle = NULL;
}

void Player::EnterVehicle(Vehicle* nearestVehicle)
{
	if (nearestVehicle != nullptr) {
		printf("WE ARE IN VEHICLE");
		vehicle = nearestVehicle;
	}
}

Vehicle* Player::GetCurrentVehicle()
{
	return vehicle;
}

void Player::addToInventory(uint32_t slot, uint32_t ammo)
{
	weapons[slot].weaponId = slot;
	weapons[slot].bulletsTotal += ammo;
}

void Player::setActiveWeapon(uint32_t slot)
{
	currentWeapon = slot;
}

void Player::Jump()
{
	if (physCharacter->onGround()) physCharacter->jump(btVector3(0.0f, 0.0f, 30.0f));
}

void Player::Draw()
{
	player->Draw();
}