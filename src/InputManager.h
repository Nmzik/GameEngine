#pragma once
#include <memory>
#include <vector>

#include "InputActions.h"

#include <glm/gtc/type_precision.hpp>

class InputManager
{
    uint8_t currentInput[ActionsKeySize];
    uint8_t previousInput[ActionsKeySize];
    glm::i32vec2 mouseMovement;

public:
    InputManager();
    ~InputManager();

    void processButton(Actions key, bool isDown);
    void update();

    bool isKeyTriggered(Actions iKeyCode) const;
    bool isKeyPressed(Actions iKeyCode) const;
    bool isKeyReleased(Actions iKeyCode) const;

    void setMouseMovement(int x, int y);
    glm::i32vec2 getMouseMovement() const;
};
