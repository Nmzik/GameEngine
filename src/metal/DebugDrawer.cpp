#include "DebugDrawer.h"

DebugDrawer::DebugDrawer()
{
}

DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
}

void DebugDrawer::drawContactPoint(
    const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void DebugDrawer::setDebugMode(int debugMode)
{
    this->debugMode = debugMode;
}

int DebugDrawer::getDebugMode() const
{
    return debugMode;
}

void DebugDrawer::render()
{
    
}
