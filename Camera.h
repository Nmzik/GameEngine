#pragma once
#ifndef CAMERA_H
#define CAMERA_H

//#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 100.0f;
const float SENSITIVTY = 1.0f;
const float ZOOM = 25.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
	class ViewPlane {
	public:
		glm::vec3 normal{};
		float distance{};
	};
public:
	ViewPlane planes[6];
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(2137, 3656, 100), glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		//printf("POSITION %f, %f, %f\n",Position.x, Position.y, Position.z);
		//printf("TEST %f, %f, %f\n", Position.x + Front.x, Position.y + Front.y, Position.z + Front.z);
		return glm::lookAt(Position - Front * 7.0f, Position, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Pitch += xoffset;
		Yaw += yoffset;
		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Yaw < -70.0f)
				Yaw = -70.0f;
			if (Yaw > 20.0f)
				Yaw = 20.0f;
		}
		//printf("PITCH %f\n", Pitch);
		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

	void UpdateFrustum(const glm::mat4& proj) {

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

	bool intersects(glm::vec3 center, float radius) const {

		for (const auto &plane : planes) {
			if (glm::dot(plane.normal, center) + plane.distance < -radius)
				return false;
		}

		return true;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.z = sin(glm::radians(Yaw));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif

