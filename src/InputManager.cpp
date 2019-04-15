#include "InputManager.h"

InputManager::InputManager()
{
#if (WIN32)
    keyMapping.emplace_back(0x1B, button_ESCAPE);
    keyMapping.emplace_back(0x45, Actions::button_E);
    keyMapping.emplace_back(0x42, Actions::button_player1);
    keyMapping.emplace_back(0x4E, Actions::button_player2);
    keyMapping.emplace_back(0x4D, Actions::button_player3);
    keyMapping.emplace_back(0x71, Actions::button_ShowCollision);
    keyMapping.emplace_back(0x46, Actions::button_CameraMode);
    keyMapping.emplace_back(0x55, Actions::button_EnterExitVehicle);
    keyMapping.emplace_back(0x57, Actions::button_Forward);
    keyMapping.emplace_back(0x41, Actions::button_TurnLeft);
    keyMapping.emplace_back(0x44, Actions::button_TurnRight);
    keyMapping.emplace_back(0x53, Actions::button_Backward);
    keyMapping.emplace_back(0x51, Actions::button_CameraUp);
    keyMapping.emplace_back(0x5A, Actions::button_CameraDown);
    keyMapping.emplace_back(0xA0, button_LSHIFT);
    keyMapping.emplace_back(0x20, button_SPACE);
#endif
}

InputManager::~InputManager()
{
}

void InputManager::ProcessButton(uint32_t key, bool isDown)
{
    for (int i = 0; i < keyMapping.size(); i++)
    {
        if (key == keyMapping[i].first)
        {
            if (isDown)
                Input[keyMapping[i].second] = 1;
            else
                Input[keyMapping[i].second] = 0;
        }
    }
}

bool InputManager::IsKeyTriggered(Actions iKeyCode) const
{
    return (Input[iKeyCode] == 1);
}

bool InputManager::IsKeyPressed(Actions iKeyCode) const
{
    return (Input[iKeyCode] == 1);
}

bool InputManager::IsKeyReleased(Actions iKeyCode) const
{
    return (Input[iKeyCode] == 0);
}

void InputManager::setMouseMovement(int x, int y)
{
    mouseMovement.x = x;
    mouseMovement.y = y;
}

glm::i32vec2 InputManager::getMouseMovement() const
{
    return mouseMovement;
}
