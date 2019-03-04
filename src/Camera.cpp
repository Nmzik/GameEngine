#include "Camera.h"

#include "CPed.h"
#include "CVehicle.h"

void Camera::updateThirdPersonCamera()
{
    CPed* player = world.peds[world.currentPlayerID];

    glm::vec3 targetPosition;

    //Player in vehicle
    if (player->getCurrentVehicle())
    {
        targetPosition = player->getCurrentVehicle()->getPosition();

        //position = glm::vec3(vehiclePosition.getX(),
        //     vehiclePosition.getY() - 5.f,
        //   vehiclePosition.getZ() + 5.0f);
    }
    else
    {
        targetPosition = player->getPosition();
    }

    auto look = glm::vec2(lookcamera.x * 0.01f, lookcamera.y * 0.01f);
    // Determine the "ideal" camera position for the current view angles
    auto yaw = glm::angleAxis(-look.x - glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f));
    auto pitch = glm::angleAxis(look.y, glm::vec3(0.f, 1.f, 0.f));
    auto cameraOffset = yaw * pitch * glm::vec3(0.f, 0.f, 5.0f);
    glm::vec3 cameraPosition = targetPosition + cameraOffset;

    glm::vec3 lookTargetPosition(targetPosition);
    targetPosition += glm::vec3(0.f, 0.f, 1.f);
    lookTargetPosition += glm::vec3(0.f, 0.f, 0.5f);

    auto lookdir = glm::normalize(lookTargetPosition - cameraPosition);
    // Calculate the angles to look at the target position
    float len2d = glm::length(glm::vec2(lookdir));
    float anglePitch = glm::atan(lookdir.z, len2d);
    float angleYaw = glm::atan(lookdir.y, lookdir.x);
    glm::quat angle(glm::vec3(0.f, -anglePitch, angleYaw));

    position = cameraPosition;
    rotation = angle;

    auto up = rotation * glm::vec3(0.f, 0.f, 1.f);
    ViewMatrix = glm::lookAt(position, position + rotation * glm::vec3(1.f, 0.f, 0.f), up);
}

void Camera::updateFreeCamera()
{
    auto look = glm::vec2(lookcamera.x * 0.01f, lookcamera.y * 0.01f);
    // Determine the "ideal" camera position for the current view angles
    auto yaw = glm::angleAxis(-look.x - glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f));
    auto pitch = glm::angleAxis(look.y, glm::vec3(0.f, 1.f, 0.f));
    auto cameraOffset = yaw * pitch * glm::vec3(0.f, 0.f, 5.0f);
    glm::vec3 cameraPosition = position + cameraOffset;

    glm::vec3 lookTargetPosition(position);
    //position += glm::vec3(0.f, 0.f, 1.f);
    lookTargetPosition += glm::vec3(0.f, 0.f, 0.5f);

    auto lookdir = glm::normalize(lookTargetPosition - cameraPosition);
    // Calculate the angles to look at the target position
    float len2d = glm::length(glm::vec2(lookdir));
    float anglePitch = glm::atan(lookdir.z, len2d);
    float angleYaw = glm::atan(lookdir.y, lookdir.x);
    glm::quat angle(glm::vec3(0.f, -anglePitch, angleYaw));
    rotation = angle;

    auto up = rotation * glm::vec3(0.f, 0.f, 1.f);
    ViewMatrix = glm::lookAt(position, position + rotation * glm::vec3(1.f, 0.f, 0.f), up);
}

void Camera::onUpdate()
{
    switch (cameraMode)
    {
        case CameraMode::ThirdPerson:
            updateThirdPersonCamera();
            break;
        case CameraMode::FirstPerson:
            printf("NOT IMPLEMENTED");
            break;
        case CameraMode::FreeCamera:
            updateFreeCamera();
        default:
            break;
    }
}

void Camera::updateFrustum(const glm::mat4& proj)
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

bool Camera::intersects(glm::vec3 center, float radius) const
{
    for (const auto& plane : planes)
    {
        if (glm::dot(plane.normal, center) + plane.distance < -radius)
            return false;
    }

    return true;
}
