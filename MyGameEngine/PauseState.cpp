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