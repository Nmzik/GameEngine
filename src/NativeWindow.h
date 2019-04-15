#pragma once
#include <iostream>
#include <sstream>
#include "InputManager.h"

class NativeWindow
{
    InputManager* inputManager;

public:
    NativeWindow() = default;
    virtual ~NativeWindow() = default;

    void AddListener(InputManager* input)
    {
        inputManager = input;
    }

    void ProcessButtonPress(uint32_t key, bool isDown)
    {
        inputManager->ProcessButton(key, isDown);
    }

    void ProcessMouseEvents(int x, int y)
    {
        inputManager->setMouseMovement(x, y);
    }

    virtual void InitializeContext() = 0;
    virtual void setTitle(std::string& osstr) = 0;

    virtual void ProcessEvents() = 0;

    virtual void SwapBuffers() = 0;
};
