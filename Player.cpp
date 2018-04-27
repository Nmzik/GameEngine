#include "Player.h"

Player::Player(glm::vec3 position, YddLoader* ydd, btDiscreteDynamicsWorld* world) : player(ydd), World(world)
{
	btScalar mass(1.0f);
	btVector3 localInertia(0, 0, 0);

	physShape = new btCapsuleShapeZ(0.45f, 1.2f);

	physShape->calculateLocalInertia(mass, localInertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, physShape, localInertia);
	rbInfo.m_restitution = 0.0f;
	rbInfo.m_friction = 1.0;

	body = new btRigidBody(rbInfo);
	body->setSleepingThresholds(0.0, 0.0);
	body->setAngularFactor(0.0);
	body->forceActivationState(DISABLE_DEACTIVATION);

	world->addRigidBody(body, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter);



	playerDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

Player::~Player()
{
	delete body->getMotionState();

	delete physShape;

	World->removeRigidBody(body);

	delete body;
}

btRigidBody* Player::getPhysCharacter()
{
	return body;
}

void Player::TakeDamage(float dmg)
{
	health -= dmg;
}

glm::mat4& Player::getPosition()
{
	//printf("POS %f\n", physCharacter->getGhostObject()->getWorldTransform().getOrigin().getZ());
	if (body->getWorldTransform().getOrigin().getZ() <= -150) {
		body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), body->getWorldTransform().getOrigin() + btVector3(0, 0, 300)));
	}

	body->getWorldTransform().getOpenGLMatrix(&model[0][0]);

	return model;
}

void Player::PhysicsTick()
{
	

	//body->setLinearVelocity(btVector3(playerDirection.x, playerDirection.y, playerDirection.z));
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
	btVector3 m_rayStart = body->getCenterOfMassPosition();
	btVector3 m_rayEnd = m_rayStart - btVector3(0.0, 0.0, 1.5);

	// rayCallback
	btCollisionWorld::ClosestRayResultCallback rayCallback(m_rayStart, m_rayEnd);

	World->rayTest(m_rayStart, m_rayEnd, rayCallback);
	if (rayCallback.hasHit())
	{//JUMP!
		body->applyCentralImpulse(btVector3(0.f, 0.f, 40.0f));
	}
}

void Player::Draw(Shader* shader)
{
	player->Draw(shader);
}