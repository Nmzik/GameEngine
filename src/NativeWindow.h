#pragma once
#include "InputManager.h"

class NativeWindow
{
    InputManager* inputManager;

public:
    NativeWindow() = default;
    virtual ~NativeWindow() = default;

    void addListener(InputManager* input)
    {
        inputManager = input;
    }

    void processButtonEvent(uint32_t key, bool isDown)
    {
        inputManager->processButton(key, isDown);
    }

    void processMouseEvents(int x, int y)
    {
        inputManager->setMouseMovement(x, y);
    }

    virtual void initializeContext() = 0;
    virtual void setTitle(std::string& osstr) = 0;

    virtual void processEvents() = 0;

    virtual void swapBuffers() = 0;
};
