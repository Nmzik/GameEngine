#include "InGameState.h"
#include "MenuState.h"
#include "Game.h"

InGameState::InGameState(Game* game)
	: State(game)
{
}

InGameState::~InGameState()
{
}

void InGameState::enter()
{
	printf("WE ARE INGAME\n");
}

void InGameState::exit()
{
	printf("WE ARE OUT OF INGAME\n");
}
