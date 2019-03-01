#include "PhysicsDebugDrawer.h"

PhysicsDebugDrawer::PhysicsDebugDrawer()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboPositions);

    glGenBuffers(1, &m_vboColors);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    m_points.push_back(glm::vec3(from.getX(), from.getY(), from.getZ()));
    m_colors.push_back(glm::vec3(color.getX(), color.getY(), color.getZ()));
    m_points.push_back(glm::vec3(to.getX(), to.getY(), to.getZ()));
    m_colors.push_back(glm::vec3(color.getX(), color.getY(), color.getZ()));
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
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(glm::vec3), &m_points[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), &m_colors[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_LINES, 0, m_points.size());

    m_points.clear();
    m_colors.clear();
}
