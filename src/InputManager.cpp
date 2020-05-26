#include "InputManager.h"

#if (WIN32)
#include "SDL.h"
#endif

InputManager::InputManager()
    : mouseMovement({0, 0})
{
    memset(&currentInput[0], 0, ActionsKeySize);
    memset(&previousInput[0], 0, ActionsKeySize);
#if (WIN32)  //IF SDL
#else
    /*keyMapping.emplace_back(53, Actions::button_ESCAPE);
    keyMapping.emplace_back(14, Actions::button_E);
    keyMapping.emplace_back(11, Actions::button_player1);
    keyMapping.emplace_back(45, Actions::button_player2);
    keyMapping.emplace_back(46, Actions::button_player3);
    keyMapping.emplace_back(0x71, Actions::button_ShowCollision);
    keyMapping.emplace_back(3, Actions::button_CameraMode);
    keyMapping.emplace_back(0x34, Actions::button_EnterExitVehicle);
    keyMapping.emplace_back(13, Actions::button_Forward);
    keyMapping.emplace_back(0, Actions::button_TurnLeft);
    keyMapping.emplace_back(2, Actions::button_TurnRight);
    keyMapping.emplace_back(1, Actions::button_Backward);
    keyMapping.emplace_back(12, Actions::button_CameraUp);
    keyMapping.emplace_back(6, Actions::button_CameraDown);
    keyMapping.emplace_back(56, Actions::button_LSHIFT);
    keyMapping.emplace_back(49, Actions::button_SPACE);*/
#endif
}

InputManager::~InputManager()
{
}

void InputManager::processButton(Actions key, bool isDown)
{
    currentInput[key] = (uint8_t)isDown;
}

void InputManager::update()
{
    memcpy(&previousInput[0], &currentInput[0], ActionsKeySize);
}

bool InputManager::isKeyTriggered(Actions iKeyCode) const
{
    return (currentInput[iKeyCode] == 1 && previousInput[iKeyCode] == 0);
}

bool InputManager::isKeyPressed(Actions iKeyCode) const
{
    return (currentInput[iKeyCode] == 1);
}

bool InputManager::isKeyReleased(Actions iKeyCode) const
{
    return (currentInput[iKeyCode] == 0 && previousInput[iKeyCode] == 1);
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
