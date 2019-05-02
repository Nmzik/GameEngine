#pragma once
#include <cstring>
#include <memory>
#include <vector>
#include "InputActions.h"
#include "glm/glm.hpp"

class InputManager
{
    uint8_t CurrentInput[ActionsKeySize];
    uint8_t PreviousInput[ActionsKeySize];
    glm::i32vec2 mouseMovement;
    std::vector<std::pair<uint32_t, Actions>> keyMapping;

public:
    InputManager();
    ~InputManager();

    void ProcessButton(uint32_t key, bool isDown);
    void update();

    bool IsKeyTriggered(Actions iKeyCode) const;
    bool IsKeyPressed(Actions iKeyCode) const;
    bool IsKeyReleased(Actions iKeyCode) const;

    void setMouseMovement(int x, int y);
    glm::i32vec2 getMouseMovement() const;
};
