#include "NativeWindow.h"

NativeWindow::NativeWindow()
{
	SDL_Init(SDL_INIT_VIDEO);  // Initialize SDL2

	window = SDL_CreateWindow("SDL2 window",            // window title
		SDL_WINDOWPOS_UNDEFINED,  // initial x position
		SDL_WINDOWPOS_UNDEFINED,  // initial y position
		1280,                     // width, in pixels
		720,                      // height, in pixels
		SDL_WINDOW_OPENGL         //| SDL_WINDOW_FULLSCREEN                 // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL)
	{
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);
}


NativeWindow::~NativeWindow()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void NativeWindow::setTitle(std::ostringstream& osstr)
{
    SDL_SetWindowTitle(window, osstr.str().c_str());
}
