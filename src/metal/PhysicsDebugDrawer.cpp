#include "PhysicsDebugDrawer.h"

PhysicsDebugDrawer::PhysicsDebugDrawer()
{
    
}

PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
   
}

void PhysicsDebugDrawer::drawContactPoint(
    const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
    this->debugMode = debugMode;
}

int PhysicsDebugDrawer::getDebugMode() const
{
    return debugMode;
}

void PhysicsDebugDrawer::render()
{
    
}
