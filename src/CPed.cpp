#include "CPed.h"

#include "CVehicle.h"

#include "PhysicsSystem.h"
#include "YddLoader.h"
#include "YdrLoader.h"

CPed::CPed(glm::vec3 position, YddLoader* ydd)
    : vehicle(nullptr)
    , player(ydd)
    , Entity(position, glm::quat(-1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f))
{
    health = 200;
    playerModel.push_back(ydd->ydrFiles[121241095].get());
    playerModel.push_back(ydd->ydrFiles[1471150075].get());
    playerModel.push_back(ydd->ydrFiles[2540683012].get());

    btScalar mass(1.0f);
    btVector3 localInertia(0, 0, 0);

    physShape = std::make_unique<btCapsuleShapeZ>(0.45f, 1.2f);
    physShape->calculateLocalInertia(mass, localInertia);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(position.x, position.y, position.z));

    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, physShape.get(), localInertia);
    rbInfo.m_restitution = 0.0f;
    rbInfo.m_friction = 1.0;

    body = std::make_unique<btRigidBody>(rbInfo);

    body->setUserPointer(this);

    body->setSleepingThresholds(0.0, 0.0);
    body->setAngularFactor(0.0);
    body->forceActivationState(DISABLE_DEACTIVATION);

    PhysicsSystem::dynamicsWorld->addRigidBody(body.get(),
                                               btBroadphaseProxy::KinematicFilter,
                                               btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter | btBroadphaseProxy::DefaultFilter);

    playerDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

CPed::~CPed()
{
    PhysicsSystem::dynamicsWorld->removeRigidBody(body.get());
    delete body->getMotionState();
}

btRigidBody* CPed::getPhysCharacter()
{
    return body.get();
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

glm::mat4& CPed::getMatrix()
{
    body->getWorldTransform().getOpenGLMatrix(&modelMatrix[0][0]);

    return modelMatrix;
}

void CPed::physicsTick()
{
    position = glm::vec3(body->getWorldTransform().getOrigin().x(), body->getWorldTransform().getOrigin().y(), body->getWorldTransform().getOrigin().z());
    //	body->setLinearVelocity(btVector3(playerDirection.x, playerDirection.y, playerDirection.z));
}

void CPed::exitVehicle()
{
    vehicle->setThrottle(0.0f);
    vehicle->setSteeringValue(0.0f);
    vehicle = NULL;
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
