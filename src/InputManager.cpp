#include "InputManager.h"

InputManager::InputManager()
{
    memset(PreviousInput, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(CurrentInput, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
    SDL_PumpEvents();
    memcpy(PreviousInput, CurrentInput, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(CurrentInput, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

bool InputManager::IsKeyTriggered(const SDL_Scancode iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 1 && PreviousInput[iKeyCode] == 0);
}

bool InputManager::IsKeyPressed(const SDL_Scancode iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 1);
}

bool InputManager::IsKeyReleased(const SDL_Scancode iKeyCode) const
{
    return (CurrentInput[iKeyCode] == 0 && PreviousInput[iKeyCode] == 1);
}
