#include "PhysicsSystem.h"

#include "CPed.h"
#include "CVehicle.h"

FreeListAllocator* physicsAllocator;

#if (WIN32)
#define Allocator_API __declspec(allocator)
#include <VSCustomNativeHeapEtwProvider.h>
auto pHeapTracker = std::make_unique<VSHeapTracker::CHeapTracker>("PhysicsHeap");
#else
#define Allocator_API
#endif

Allocator_API void* allocate(size_t size, int alignment)
{
    void* ptr = physicsAllocator->allocate(size, (uint8_t)alignment);
#if (WIN32)
    pHeapTracker->AllocateEvent(ptr, size);
#endif
    return ptr;
}
void deallocate(void* memblock)
{
#if (WIN32)
    pHeapTracker->DeallocateEvent(memblock);
#endif
    physicsAllocator->deallocate(memblock);
}

PhysicsSystem::PhysicsSystem()
{
    physicsAllocator = new FreeListAllocator(50 * 1024 * 1024);

    btAlignedAllocSetCustomAligned(allocate, deallocate);

    broadphase = std::make_unique<btDbvtBroadphase>();
    collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());

    dynamicsWorld->setGravity(btVector3(0, 0, -9.8f));
    //	UPDATE STATIC OBJECTS MANUALLY
    dynamicsWorld->setForceUpdateAllAabbs(false);

    debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    dynamicsWorld->setDebugDrawer(&debug);
}

PhysicsSystem::~PhysicsSystem()
{
    //delete physicsAllocator;
}

void PhysicsSystem::update(float delta_time)
{
    dynamicsWorld->stepSimulation(delta_time, 10);

    /*if (delta_time > 0.25f) {
	 delta_time = 0.25f;
	}

	accumulatedTime += delta_time;

	while (accumulatedTime >= deltaTime) {
	 dynamicsWorld->stepSimulation(deltaTime, 2, deltaTime);

	 int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	 for (int i = 0; i < numManifolds; ++i)
	 {
	  btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
	  btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
	  btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

	  int numContacts = contactManifold->getNumContacts();
	  for (int j = 0; j < numContacts; j++)
	  {

	   btManifoldPoint& pt = contactManifold->getContactPoint(j);
	   if (pt.getAppliedImpulse() > 1) {

	 Player* player1 = static_cast<Player*>(obA->getUserPointer());
	 Player* player2 = static_cast<Player*>(obB->getUserPointer());

	 if (player1 != nullptr) {
	  player1->TakeDamage(pt.getAppliedImpulse());
	 }
	 else if (player2 != nullptr) {
	  player2->TakeDamage(pt.getAppliedImpulse());
	 }
	   }
	   /*if (pt.getDistance()<0.f)
	   {
	 if (pt.getAppliedImpulse() != 0) {
	  printf("");
	 }

	   }
	  }

	 }

	 accumulatedTime -= deltaTime;
	}*/
}

PhysicsSystem::RayResult PhysicsSystem::rayCast(glm::vec3& from, glm::vec3& to) const
{
    btVector3 rayFrom = btVector3(from.x, from.y, from.z);
    btVector3 rayTo = btVector3(to.x, to.y, to.z);

    // rayCallback
    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

    dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);

    RayResult result;
    result.HasHit = rayCallback.hasHit();
    if (rayCallback.hasHit())
    {
        result.HitPos = glm::vec3(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z());
        result.HitNormal = glm::vec3(rayCallback.m_hitNormalWorld.x(), rayCallback.m_hitNormalWorld.y(), rayCallback.m_hitNormalWorld.z());
    }

    return result;
}

void PhysicsSystem::addPed(CPed* ped)
{
    dynamicsWorld->addRigidBody(ped->getPhysCharacter(),
                                btBroadphaseProxy::KinematicFilter,
                                btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter | btBroadphaseProxy::DefaultFilter);
}

void PhysicsSystem::removePed(CPed* ped)
{
    dynamicsWorld->removeRigidBody(ped->getPhysCharacter());
}

void PhysicsSystem::addVehicle(CVehicle* vehicle)
{
    dynamicsWorld->addRigidBody(
        vehicle->getCarChassisRigidbody(), btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter);

    dynamicsWorld->addAction(vehicle->getRaycastVehicle());
}

void PhysicsSystem::removeVehicle(CVehicle* vehicle)
{
    dynamicsWorld->removeAction(vehicle->getRaycastVehicle());
    dynamicsWorld->removeRigidBody(vehicle->getCarChassisRigidbody());
}

void PhysicsSystem::addRigidBody(btRigidBody* body)
{
    dynamicsWorld->addRigidBody(body);
}

void PhysicsSystem::removeRigidBody(btRigidBody* body)
{
    dynamicsWorld->removeRigidBody(body);
}

void PhysicsSystem::putRigidBodyToSleep(btRigidBody* body)
{
    body->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::DefaultFilter;
    body->setGravity(btVector3(0.f, 0.f, 0.f));
}

void PhysicsSystem::wakeRigidBodyFromSleep(btRigidBody* body)
{
    body->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter;
    body->setGravity(dynamicsWorld->getGravity());
}
