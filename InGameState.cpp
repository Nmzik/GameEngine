#include "InGameState.h"
#include "MenuState.h"
#include "Game.h"

InGameState::InGameState(Game* game) : State(game)
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

void InGameState::tick(float delta_time)
{
	static float clockAccumulator = 0.f;

	clockAccumulator += delta_time;
	while (clockAccumulator >= 1.f) {
		game->getWorld()->gameMinute++;
		if (game->getWorld()->gameMinute >= 60) {
			game->getWorld()->gameMinute = 0;
			game->getWorld()->gameHour++;
			if (game->getWorld()->gameHour >= 24) {
				game->getWorld()->gameHour = 0;
			}
		}
		clockAccumulator -= 1.f;
	}

	//printf("Time %d %d\n", game->getWorld()->gameHour, game->getWorld()->gameMinute);

	static bool DebugPressed = true;

	//KEYBOARD
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_N]) {
		game->getRenderer()->hdrEnabled = 1;
	}

	if (state[SDL_SCANCODE_M]) {
		game->getRenderer()->hdrEnabled = 0;
	}

	if (state[SDL_SCANCODE_Q]) {
		game->getRenderer()->setType(0);
	}

	if (state[SDL_SCANCODE_E]) {
		game->getRenderer()->setType(1);
	}

	if (state[SDL_SCANCODE_Z]) {
		game->getRenderer()->sunDirection.z += 0.1f;
	}

	if (state[SDL_SCANCODE_X]) {
		game->getRenderer()->sunDirection.z -= 0.1f;
	}

	if (state[SDL_SCANCODE_C]) {
		game->getRenderer()->sunDirection.x += 0.01f;
	}

	if (state[SDL_SCANCODE_V]) {
		game->getRenderer()->sunDirection.x -= 0.01f;
	}

	if (state[SDL_SCANCODE_F]) {
		DebugPressed = true;
	}

	if (state[SDL_SCANCODE_G]) {
		DebugPressed = false;
	}
	if (DebugPressed) {
		if (state[SDL_SCANCODE_W]) game->getRenderer()->getCamera().ProcessKeyboard(FORWARD, delta_time);
		if (state[SDL_SCANCODE_S]) game->getRenderer()->getCamera().ProcessKeyboard(BACKWARD, delta_time);
		if (state[SDL_SCANCODE_A]) game->getRenderer()->getCamera().ProcessKeyboard(LEFT, delta_time);
		if (state[SDL_SCANCODE_D]) game->getRenderer()->getCamera().ProcessKeyboard(RIGHT, delta_time);
		game->getWorld()->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
	}
	else {

		//NEED PROPER FIX
		game->getRenderer()->getCamera().Position = glm::vec3(game->getWorld()->player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), game->getWorld()->player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), game->getWorld()->player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ()) + glm::vec3(0.0f,0.0f,-5.f);
		glm::vec3 movement;
		movement.x = state[SDL_SCANCODE_W] - state[SDL_SCANCODE_S];
		movement.y = state[SDL_SCANCODE_A] - state[SDL_SCANCODE_D];

		float length = glm::length(movement);
		if (length > 0.1f) {
			auto move = 1.f * glm::normalize(movement);
			game->getWorld()->player->getPhysCharacter()->setWalkDirection(btVector3(move.y, 0.f, move.x));
		}
		else {
			game->getWorld()->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
		}

		if (state[SDL_SCANCODE_SPACE]) game->getWorld()->player->Jump();
	}

	//MOUSE
	int x;
	int y;
	SDL_GetRelativeMouseState(&x, &y);
	game->getRenderer()->getCamera().ProcessMouseMovement(x, -y);

}

void InGameState::handleEvent(const SDL_Event & event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE: {
			StateManager::get().enter<PauseState>(game);
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}
}
