#include "InputSystem.h"

InputSystem::InputSystem(RenderingSystem* parent, GameWorld* world) : _RenderingSystem(parent), _world(world)
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
			if (event.key.keysym.sym == SDLK_w) _RenderingSystem->getCamera().ProcessKeyboard(FORWARD, delta_time);
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 1.f));
			//else 
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
			if (event.key.keysym.sym == SDLK_s) _RenderingSystem->getCamera().ProcessKeyboard(BACKWARD, delta_time);
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, -1.f));
			//else
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
			if (event.key.keysym.sym == SDLK_a) _RenderingSystem->getCamera().ProcessKeyboard(LEFT, delta_time);
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(-1.f, 0.f, 0.f));
			//else
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
			if (event.key.keysym.sym == SDLK_d) _RenderingSystem->getCamera().ProcessKeyboard(RIGHT, delta_time);
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(1.f, 0.f, 0.f));
			//else
				//_world->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));*/
			break;
		}
		case SDL_MOUSEMOTION: {
			_RenderingSystem->getCamera().ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		}
	}
	//keyboard handling todo
}