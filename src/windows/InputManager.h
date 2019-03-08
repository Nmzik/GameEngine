#pragma once
#include <cstring>
#include <memory>
#include <vector>
#include "InputActions.h"
#include "SDL.h"
#include "glm/glm.hpp"

class InputManager
{
    uint8_t PreviousInput[ActionsKeySize];
    uint8_t CurrentInput[ActionsKeySize];

    std::vector<std::pair<SDL_Scancode, Actions>> keyMapping;

public:
    InputManager();
    ~InputManager();

    void Update();

    bool IsKeyTriggered(Actions iKeyCode) const;
    bool IsKeyPressed(Actions iKeyCode) const;
    bool IsKeyReleased(Actions iKeyCode) const;

    glm::i32vec2 getMouseMovement() const;
};
