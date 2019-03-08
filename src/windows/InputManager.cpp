#include "InputManager.h"

InputManager::InputManager()
{
    keyMapping.emplace_back(SDL_SCANCODE_F1, button_GPU_TIME);
    keyMapping.emplace_back(SDL_SCANCODE_ESCAPE, button_ESCAPE);
    keyMapping.emplace_back(SDL_SCANCODE_E, Actions::button_E);
    keyMapping.emplace_back(SDL_SCANCODE_B, Actions::button_player1);
    keyMapping.emplace_back(SDL_SCANCODE_N, Actions::button_player2);
    keyMapping.emplace_back(SDL_SCANCODE_M, Actions::button_player3);
    keyMapping.emplace_back(SDL_SCANCODE_F2, Actions::button_ShowCollision);
    keyMapping.emplace_back(SDL_SCANCODE_F, Actions::button_CameraMode);
    keyMapping.emplace_back(SDL_SCANCODE_U, Actions::button_EnterExitVehicle);
    keyMapping.emplace_back(SDL_SCANCODE_W, Actions::button_Forward);
    keyMapping.emplace_back(SDL_SCANCODE_A, Actions::button_TurnLeft);
    keyMapping.emplace_back(SDL_SCANCODE_D, Actions::button_TurnRight);
    keyMapping.emplace_back(SDL_SCANCODE_S, Actions::button_Backward);
    keyMapping.emplace_back(SDL_SCANCODE_Q, Actions::button_CameraUp);
    keyMapping.emplace_back(SDL_SCANCODE_Z, Actions::button_CameraDown);
    keyMapping.emplace_back(SDL_SCANCODE_LSHIFT, button_LSHIFT);
    keyMapping.emplace_back(SDL_SCANCODE_SPACE, button_SPACE);

    memset(PreviousInput, 0, sizeof(Uint8) * ActionsKeySize);
    memcpy(CurrentInput, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * ActionsKeySize);
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
    SDL_PumpEvents();
    memcpy(PreviousInput, CurrentInput, sizeof(Uint8) * ActionsKeySize);

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < keyMapping.size(); i++)
    {
        if (state[keyMapping[i].first])
            CurrentInput[keyMapping[i].second] = 1;
        else
            CurrentInput[keyMapping[i].second] = 0;
    }
}

bool InputManager::IsKeyTriggered(Actions iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 1 && PreviousInput[iKeyCode] == 0);
}

bool InputManager::IsKeyPressed(Actions iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 1);
}

bool InputManager::IsKeyReleased(Actions iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 0 && PreviousInput[iKeyCode] == 1);
}

glm::i32vec2 InputManager::getMouseMovement() const
{
    glm::i32vec2 mouseMovement;
    SDL_GetMouseState(&mouseMovement.x, &mouseMovement.y);
    return mouseMovement;
}