#include "CPed.h"

#include "CVehicle.h"
#include "PhysicsSystem.h"

#include "loaders/YddLoader.h"
#include "loaders/YdrLoader.h"

CPed::CPed(glm::vec3 position, YddLoader* ydd)
    : CEntity(position, glm::quat(0.0f, 0.0f, 0.0f, -1.0f), glm::vec3(1.0f), ObjectType::Ped)
    , player(ydd)
    , vehicle(nullptr)
    , movementDirection(0, 0, 0)
{
    health = 200;

    btScalar mass(1.0f);
    btVector3 localInertia(0, 0, 0);

    physShape = std::make_unique<btCapsuleShapeZ>(0.45f, 1.2f);
    physShape->calculateLocalInertia(mass, localInertia);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    transform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, physShape.get(), localInertia);
    rbInfo.m_restitution = 0.0f;
    rbInfo.m_friction = 1.0;

    body = std::make_unique<btRigidBody>(rbInfo);

    body->setUserPointer(this);

    body->setSleepingThresholds(0.0, 0.0);
    body->setAngularFactor(0.0);
    body->forceActivationState(DISABLE_DEACTIVATION);

    playerDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

CPed::~CPed()
{
    delete body->getMotionState();
}

void CPed::initDrawable()
{
    playerModel.push_back(player->ydrFiles[121241095]);
    playerModel.push_back(player->ydrFiles[1471150075]);
    playerModel.push_back(player->ydrFiles[2540683012]);
}

void CPed::setPosition(glm::vec3 pos)
{
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

    body->setWorldTransform(transform);
}

void CPed::takeDamage(float dmg)
{
    if (dmg > health)
        health = 0;
    else
        health -= dmg;
}

void CPed::physicsTick()
{
    body->getWorldTransform().getOpenGLMatrix(&modelMatrix[0][0]);
    btScalar zValue = needsReset ? 0 : body->getLinearVelocity().z();
    body->setLinearVelocity(btVector3(movementDirection.x, movementDirection.y, zValue));
    position = glm::vec3(body->getWorldTransform().getOrigin().x(), body->getWorldTransform().getOrigin().y(), body->getWorldTransform().getOrigin().z());

	needsReset = false;
}

void CPed::exitVehicle()
{
    vehicle->setThrottle(0.0f);
    vehicle->setSteeringValue(0.0f);
    vehicle = nullptr;
}

void CPed::enterVehicle(CVehicle* nearestVehicle)
{
    if (nearestVehicle != nullptr)
    {
        printf("WE ARE IN VEHICLE\n");
        vehicle = nearestVehicle;
    }
}

void CPed::addToInventory(uint32_t slot, uint32_t ammo)
{
    weapons[slot].weaponId = slot;
    weapons[slot].bulletsTotal += ammo;
}

void CPed::setActiveWeapon(uint32_t slot)
{
    currentWeapon = slot;
}

void CPed::jump()
{
    /*btVector3 m_rayStart = body->getCenterOfMassPosition();
	btVector3 m_rayEnd = m_rayStart - btVector3(0.0, 0.0, 1.5);

	// rayCallback
	btCollisionWorld::ClosestRayResultCallback rayCallback(m_rayStart, m_rayEnd);

	World->rayTest(m_rayStart, m_rayEnd, rayCallback);
	if (rayCallback.hasHit())
	{//JUMP!
	 body->applyCentralImpulse(btVector3(0.f, 0.f, 100.0f));
	}*/
}
