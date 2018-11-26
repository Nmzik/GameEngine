#pragma once
#include "LinearMath/btIDebugDraw.h"
#include "MeshManager.h"
#include "glm/glm.hpp"
#include <vector>

class PhysicsDebugDrawer : public btIDebugDraw
{
	GLuint m_vao;
	GLuint m_vboPositions;
	GLuint m_vboColors;

	std::vector<glm::vec3> m_points;
	std::vector<glm::vec3> m_colors;

public:
	PhysicsDebugDrawer();
	~PhysicsDebugDrawer();

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	void reportErrorWarning(const char* warningString) override;
	void draw3dText(const btVector3& location, const char* textString) override;
	void setDebugMode(int debugMode) override;
	int getDebugMode() const override;

	void render();

protected:
	int debugMode;
};
