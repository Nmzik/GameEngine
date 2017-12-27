#include "InputSystem.h"

InputSystem::InputSystem(RenderingSystem* parent) : _RenderingSystem(parent)
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::update(SDL_Event& event, float delta_time)
{
	static float lastX = 0;
	static float lastY = 0;

	//std::cout << "EVENT " << event.type << std::endl;

	switch (event.type)
	{
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_w)
				_RenderingSystem->getCamera().ProcessKeyboard(FORWARD, delta_time);
			if (event.key.keysym.sym == SDLK_s)
				_RenderingSystem->getCamera().ProcessKeyboard(BACKWARD, delta_time);
			if (event.key.keysym.sym == SDLK_a)
				_RenderingSystem->getCamera().ProcessKeyboard(LEFT, delta_time);
			if (event.key.keysym.sym == SDLK_d)
				_RenderingSystem->getCamera().ProcessKeyboard(RIGHT, delta_time);
			break;
		}
		case SDL_MOUSEMOTION: {
			_RenderingSystem->getCamera().ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		}
	}
	//keyboard handling todo
}