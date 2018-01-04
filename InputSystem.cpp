#include "InputSystem.h"

InputSystem::InputSystem(RenderingSystem* parent, GameWorld* world) : _RenderingSystem(parent), _world(world)
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::update(SDL_Event& event, float delta_time)
{
	
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