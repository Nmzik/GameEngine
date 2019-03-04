#pragma once
#include <iostream>
#include <sstream>
#include "SDL.h"

class NativeWindow
{
public:
    SDL_Window* window;

    NativeWindow();
    ~NativeWindow();

    void setTitle(std::ostringstream& osstr);
};
