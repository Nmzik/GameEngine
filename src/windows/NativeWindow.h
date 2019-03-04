#pragma once
#include "SDL.h"
#include <iostream>
#include <sstream>

class NativeWindow
{
public:
    SDL_Window* window;

	NativeWindow();
	~NativeWindow();

	void setTitle(std::ostringstream& osstr);
};

