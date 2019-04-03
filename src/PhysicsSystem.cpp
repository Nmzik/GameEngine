#include "PhysicsSystem.h"

std::unique_ptr<btDiscreteDynamicsWorld> PhysicsSystem::dynamicsWorld = nullptr;

FreeListAllocator* physicsAllocator;

void* allocate(size_t size, int alignment)
{
    return physicsAllocator->allocate(size, (uint8_t)alignment);
}
void deallocate(void* memblock)
{
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

    //debug.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    //dynamicsWorld->setDebugDrawer(&debug);
}

PhysicsSystem::~PhysicsSystem()
{
    //delete physicsAllocator;
    dynamicsWorld.reset();
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
