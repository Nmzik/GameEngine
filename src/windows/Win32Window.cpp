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
    //
    keyMapping.emplace_back(SDL_SCANCODE_ESCAPE, Actions::button_ESCAPE);
    keyMapping.emplace_back(SDL_SCANCODE_E, Actions::button_E);
    keyMapping.emplace_back(SDL_SCANCODE_B, Actions::button_player1);
    keyMapping.emplace_back(SDL_SCANCODE_N, Actions::button_player2);
    keyMapping.emplace_back(SDL_SCANCODE_M, Actions::button_player3);
    keyMapping.emplace_back(SDL_SCANCODE_I, Actions::button_ShowCollision);
    keyMapping.emplace_back(SDL_SCANCODE_V, Actions::button_CameraMode);
    keyMapping.emplace_back(SDL_SCANCODE_F, Actions::button_EnterExitVehicle);
    keyMapping.emplace_back(SDL_SCANCODE_W, Actions::button_Forward);
    keyMapping.emplace_back(SDL_SCANCODE_A, Actions::button_TurnLeft);
    keyMapping.emplace_back(SDL_SCANCODE_D, Actions::button_TurnRight);
    keyMapping.emplace_back(SDL_SCANCODE_S, Actions::button_Backward);
    keyMapping.emplace_back(SDL_SCANCODE_Q, Actions::button_CameraUp);
    keyMapping.emplace_back(SDL_SCANCODE_Z, Actions::button_CameraDown);
    keyMapping.emplace_back(SDL_SCANCODE_LSHIFT, Actions::button_LSHIFT);
    keyMapping.emplace_back(SDL_SCANCODE_SPACE, Actions::button_SPACE);
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
    for (auto& key : keyMapping)
    {
        processButtonEvent(key.second, (bool)state[key.first]);
    }
}

void Win32Window::swapBuffers()
{
    SDL_GL_SwapWindow(window);
}
