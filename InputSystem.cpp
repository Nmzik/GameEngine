#include "InputSystem.h"

InputSystem::InputSystem(RenderingSystem* parent, GameWorld* world) : _RenderingSystem(parent), _world(world)
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::update(SDL_Event& event, float delta_time)
{
	static bool DebugPressed = false;

	//KEYBOARD
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_F]) {
		DebugPressed = true;
	}

	if (state[SDL_SCANCODE_G]) {
		DebugPressed = false;
	}
	if (DebugPressed) {
		if (state[SDL_SCANCODE_W]) _RenderingSystem->getCamera().ProcessKeyboard(FORWARD, delta_time);
		if (state[SDL_SCANCODE_S]) _RenderingSystem->getCamera().ProcessKeyboard(BACKWARD, delta_time);
		if (state[SDL_SCANCODE_A]) _RenderingSystem->getCamera().ProcessKeyboard(LEFT, delta_time);
		if (state[SDL_SCANCODE_D]) _RenderingSystem->getCamera().ProcessKeyboard(RIGHT, delta_time);
	}
	else {
		glm::vec3 movement;
		movement.x = state[SDL_SCANCODE_W] - state[SDL_SCANCODE_S];
		movement.y = state[SDL_SCANCODE_A] - state[SDL_SCANCODE_D];

		float length = glm::length(movement);
		if (length > 0.1f) {
			auto move = 1.f * glm::normalize(movement);
			_world->player->getPhysCharacter()->setWalkDirection(btVector3(move.y, 0.f, move.x));
		}
		else {
			_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
		}

		if (state[SDL_SCANCODE_SPACE]) _world->player->Jump();
	}

	//MOUSE
	int x;
	int y;
	SDL_GetRelativeMouseState(&x, &y);
	_RenderingSystem->getCamera().ProcessMouseMovement(x, -y);
	/*switch (event.type)
	{
		case SDL_KEYDOWN: {
			
			break;
			//_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
		}
		case SDL_MOUSEMOTION: {
			_RenderingSystem->getCamera().ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		}
	}*/
}