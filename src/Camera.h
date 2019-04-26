#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameWorld.h"

enum class CameraMode
{
    ThirdPerson,
    FirstPerson,
    FreeCamera
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
    class ViewPlane
    {
    public:
        glm::vec3 normal{};
        float distance{};
    };

    glm::vec3 position;
    glm::quat rotation;

    glm::mat4 projection;
    glm::mat4 ViewMatrix;

    CameraMode cameraMode;
    ViewPlane planes[6];
public:
    // Constructor with vectors
    Camera(glm::vec3 pos, GameWorld* gameworld)
        : position(pos)
        , rotation(1.0f, 0.0f, 0.0f, 0.0f)
        , cameraMode(CameraMode::ThirdPerson)
        , projection(glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 10000.0f))
    {
    }

    glm::i32vec2 lookcamera;

    CameraMode getCameraMode() const
    {
        return cameraMode;
    }

    void setCameraMode(CameraMode _camMode)
    {
        cameraMode = _camMode;
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const
    {
        return ViewMatrix;
    }

    glm::mat4 getProjection() const
    {
        return projection;
    }

    glm::vec3 getPosition() const
    {
        return position;
    }

    void setPosition(glm::vec3 pos)
    {
        position = pos;
    }

    void updateThirdPersonCamera(CPed* player);

    void updateFreeCamera();

    void onUpdate(CPed* ped);

    void updateFrustum(const glm::mat4& proj);

    bool intersects(glm::vec3 center, float radius) const;
};
