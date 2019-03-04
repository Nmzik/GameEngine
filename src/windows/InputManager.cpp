#include "InputManager.h"


InputManager::InputManager()
{
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_ESCAPE, button_ESCAPE));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_E, Actions::button_E));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_B, Actions::button_player1));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_N, Actions::button_player2));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_M, Actions::button_player3));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_I, Actions::button_ShowCollision));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_F, Actions::button_CameraMode));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_U, Actions::button_EnterExitVehicle));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_W, Actions::button_Forward));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_A, Actions::button_TurnLeft));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_D, Actions::button_TurnRight));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_S, Actions::button_Backward));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_Q, Actions::button_CameraUp));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_Z, Actions::button_CameraDown));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_LSHIFT, button_LSHIFT));
    keyMapping.push_back(std::make_pair(SDL_SCANCODE_SPACE, button_SPACE));

    memset(PreviousInput, 0, sizeof(Uint8) * ActionsKeySize);
    memcpy(CurrentInput, SDL_GetKeyboardState(NULL), sizeof(Uint8) * ActionsKeySize);
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
    SDL_PumpEvents();
    memcpy(PreviousInput, CurrentInput, sizeof(Uint8) * ActionsKeySize);

	const Uint8* state = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < keyMapping.size(); i++)
    {
            if (state[keyMapping[i].first])
                CurrentInput[keyMapping[i].second] = 1;
            else
                CurrentInput[keyMapping[i].second] = 0;
    }
}

bool InputManager::IsKeyTriggered(const Actions iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 1 && PreviousInput[iKeyCode] == 0);
}

bool InputManager::IsKeyPressed(const Actions iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 1);
}

bool InputManager::IsKeyReleased(const Actions iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 0 && PreviousInput[iKeyCode] == 1);
}

glm::i32vec2 InputManager::getMouseMovement() const
{
    glm::i32vec2 mouseMovement;
    SDL_GetMouseState(&mouseMovement.x, &mouseMovement.y);
    return mouseMovement;
}
