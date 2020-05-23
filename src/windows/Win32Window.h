#pragma once
#include <SDL.h>

#include "../NativeWindow.h"

class Win32Window : public NativeWindow
{
    SDL_Window* window;
#if VULKAN_API

#else
    SDL_GLContext context;
#endif
    const Uint8* state;
    std::vector<std::pair<uint32_t, Actions>> keyMapping;
public:
    Win32Window();
    ~Win32Window();

    virtual void initializeContext();
    virtual void setTitle(std::string& osstr);
    virtual void processEvents();
    virtual void swapBuffers();

    SDL_Window* getSDLWindow() const
    {
        return window;
    }
};
