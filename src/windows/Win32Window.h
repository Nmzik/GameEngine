#pragma once
#include "../NativeWindow.h"
#include <SDL.h>

class Win32Window : public NativeWindow
{
    SDL_Window* window;
    SDL_GLContext context;

public:
    Win32Window();
    ~Win32Window();

    virtual void initializeContext();
    virtual void setTitle(std::string& osstr);
    virtual void processEvents();
    virtual void swapBuffers();
};
