#include "MenuState.h"
#include "Game.h"

MenuState::MenuState(Game* game) : State(game)
{

}


MenuState::~MenuState()
{

}

void MenuState::enter()
{
	printf("WE ARE MENU\n");
	game->paused = true;
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void MenuState::exit()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	game->paused = false;
	printf("WE ARE OUT OF MENU\n");
}

void MenuState::tick(float delta_time)
{

}

void MenuState::handleEvent(const SDL_Event & event)
{
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		StateManager::get().enter<InGameState>(game);
		break;
	default:
		break;
	}
}
