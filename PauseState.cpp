#include "PauseState.h"
#include "Game.h"

PauseState::PauseState(Game* game) : State(game)
{

}


PauseState::~PauseState()
{

}

void PauseState::enter()
{
	printf("WE ARE PAUSE\n");
	game->paused = true;
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void PauseState::exit()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	game->paused = false;
	printf("WE ARE OUT OF PAUSE MENU\n");
}

void PauseState::tick(float delta_time)
{

}

void PauseState::handleEvent(const SDL_Event & event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			StateManager::get().exit();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
