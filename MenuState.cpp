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
	printf("WE ARE PAUSE\n");
	game->paused = true;
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void MenuState::exit()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	game->paused = false;
	printf("WE ARE OUT OF PAUSE MENU\n");
}

void MenuState::tick(float delta_time)
{

}

void MenuState::handleEvent(const SDL_Event & event)
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
