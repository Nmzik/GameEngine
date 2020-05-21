#include "Win32Window.h"

Win32Window::Win32Window()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("Unable to initialize SDL");

#ifdef VULKAN_API
    window = SDL_CreateWindow("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1280, 720, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Create our window centered at 512x512 resolution */
    window = SDL_CreateWindow("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
#endif
    if (!window) /* Die if creation failed */
        printf("Unable to create window");

	state = SDL_GetKeyboardState(NULL);
}

Win32Window::~Win32Window()
{
}

void Win32Window::initializeContext()
{
#ifdef VULKAN_API

#else
    context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);
#endif
}

void Win32Window::setTitle(std::string& osstr)
{
}

void Win32Window::processEvents()
{
    int x, y;
    SDL_GetGlobalMouseState(&x, &y);
    processMouseEvents(x, y);

    SDL_PumpEvents();
	processButtonEvent(SDL_SCANCODE_W, state[SDL_SCANCODE_W] ? true : false);
    processButtonEvent(SDL_SCANCODE_S, state[SDL_SCANCODE_S] ? true : false);
	processButtonEvent(SDL_SCANCODE_A, state[SDL_SCANCODE_A] ? true : false);
    processButtonEvent(SDL_SCANCODE_D, state[SDL_SCANCODE_D] ? true : false);
}

void Win32Window::swapBuffers()
{
    SDL_GL_SwapWindow(window);
}
