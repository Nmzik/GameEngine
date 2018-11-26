#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
	class ViewPlane
	{
	public:
		glm::vec3 normal{};
		float distance{};
	};

public:
	ViewPlane planes[6];
	// Camera Attributes
	glm::vec3 position;
	glm::quat rotation;

	// Constructor with vectors
	Camera(glm::vec3 pos)
		: position(pos)
		, rotation(1.0f, 0.0f, 0.0f, 0.0f)
	{
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		//	printf("POSITION %f, %f, %f\n", position.x, position.y, position.z);
		auto up = rotation * glm::vec3(0.f, 0.f, 1.f);
		return glm::lookAt(position, position + rotation * glm::vec3(1.f, 0.f, 0.f), up);
	}

	void UpdateFrustum(const glm::mat4& proj)
	{

		planes[0].normal.x = proj[0][3] + proj[0][0];
		planes[0].normal.y = proj[1][3] + proj[1][0];
		planes[0].normal.z = proj[2][3] + proj[2][0];
		planes[0].distance = proj[3][3] + proj[3][0];

		auto l = glm::length(planes[0].normal);
		planes[0].normal /= l;
		planes[0].distance /= l;

		///
		planes[1].normal.x = proj[0][3] - proj[0][0];
		planes[1].normal.y = proj[1][3] - proj[1][0];
		planes[1].normal.z = proj[2][3] - proj[2][0];
		planes[1].distance = proj[3][3] - proj[3][0];

		l = glm::length(planes[1].normal);
		planes[1].normal /= l;
		planes[1].distance /= l;

		///
		planes[2].normal.x = proj[0][3] + proj[0][1];
		planes[2].normal.y = proj[1][3] + proj[1][1];
		planes[2].normal.z = proj[2][3] + proj[2][1];
		planes[2].distance = proj[3][3] + proj[3][1];

		l = glm::length(planes[2].normal);
		planes[2].normal /= l;
		planes[2].distance /= l;

		///
		planes[3].normal.x = proj[0][3] - proj[0][1];
		planes[3].normal.y = proj[1][3] - proj[1][1];
		planes[3].normal.z = proj[2][3] - proj[2][1];
		planes[3].distance = proj[3][3] - proj[3][1];

		l = glm::length(planes[3].normal);
		planes[3].normal /= l;
		planes[3].distance /= l;

		///
		planes[4].normal.x = proj[0][3] + proj[0][2];
		planes[4].normal.y = proj[1][3] + proj[1][2];
		planes[4].normal.z = proj[2][3] + proj[2][2];
		planes[4].distance = proj[3][3] + proj[3][2];

		l = glm::length(planes[4].normal);
		planes[4].normal /= l;
		planes[4].distance /= l;

		///
		planes[5].normal.x = proj[0][3] - proj[0][2];
		planes[5].normal.y = proj[1][3] - proj[1][2];
		planes[5].normal.z = proj[2][3] - proj[2][2];
		planes[5].distance = proj[3][3] - proj[3][2];

		l = glm::length(planes[5].normal);
		planes[5].normal /= l;
		planes[5].distance /= l;
	}

	bool intersects(glm::vec3 center, float radius) const
	{

		for (const auto& plane : planes)
		{
			if (glm::dot(plane.normal, center) + plane.distance < -radius)
				return false;
		}

		return true;
	}
};
#endif
