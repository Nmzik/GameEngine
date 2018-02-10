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

	if (game->getInput()->IsKeyPressed(SDL_SCANCODE_Q)) {
		game->getRenderer()->hdrEnabled = true;
	}

	if (game->getInput()->IsKeyPressed(SDL_SCANCODE_E)) {
		game->getRenderer()->hdrEnabled = false;
	}

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_Z)) {
		game->getWorld()->models[0]->xpos += 1.0f;
	}

	if (game->getInput()->IsKeyPressed(SDL_SCANCODE_X)) {
		//game->getWorld()->ydrLoader[0].z -= 0.1f;
	}

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_V)) {
		game->getWorld()->TestFunction();
	}
	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_B)) {
		game->getWorld()->ClearTestFunction();
	}
	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_M)) {
		game->getRenderer()->test = !game->getRenderer()->test;
	}
	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_I)) {
		game->getRenderer()->RenderDebugWorld = !game->getRenderer()->RenderDebugWorld;
	}

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_F)) {
		DebugPressed = !DebugPressed;
		if (DebugPressed == false) game->getWorld()->player->getPhysCharacter()->warp(btVector3(game->getRenderer()->getCamera().Position.x, game->getRenderer()->getCamera().Position.y, game->getRenderer()->getCamera().Position.z));
	}

	if (DebugPressed) {
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_W)) game->getRenderer()->getCamera().ProcessKeyboard(FORWARD, delta_time);
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_S)) game->getRenderer()->getCamera().ProcessKeyboard(BACKWARD, delta_time);
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_A)) game->getRenderer()->getCamera().ProcessKeyboard(LEFT, delta_time);
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_D)) game->getRenderer()->getCamera().ProcessKeyboard(RIGHT, delta_time);
		//game->getWorld()->player->getPhysCharacter()->warp(btVector3(game->getRenderer()->getCamera().Position.x, game->getRenderer()->getCamera().Position.y, game->getRenderer()->getCamera().Position.z));
		//game->getWorld()->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
	}
	else {

		//NEED PROPER FIX

		if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_U)) {
			if (game->getWorld()->player->GetCurrentVehicle()) {
				//in Vehicle
				printf("EXITING");
				game->getWorld()->player->getPhysCharacter()->warp(game->getWorld()->player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin());
				game->getWorld()->player->ExitVehicle();
			} else {
				printf("ENTERING");
				game->getWorld()->player->EnterVehicle(game->getWorld()->FindNearestVehicle());
			}
		}

		glm::vec3 movement;
		movement.x = game->getInput()->IsKeyPressed(SDL_SCANCODE_W) - game->getInput()->IsKeyPressed(SDL_SCANCODE_S);
		movement.z = game->getInput()->IsKeyPressed(SDL_SCANCODE_A) - game->getInput()->IsKeyPressed(SDL_SCANCODE_D);

		if (game->getWorld()->player->GetCurrentVehicle()) {
			game->getRenderer()->getCamera().Position = glm::vec3(game->getWorld()->player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getX(), game->getWorld()->player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getY(), game->getWorld()->player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getZ());
			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_W)) {
				game->getWorld()->player->GetCurrentVehicle()->SetThrottle(1.0);
			}
			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_S)) {
				game->getWorld()->player->GetCurrentVehicle()->SetThrottle(-1.0);
			}
			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_A)) {
				game->getWorld()->player->GetCurrentVehicle()->steeringValue += 0.04;
				if (game->getWorld()->player->GetCurrentVehicle()->steeringValue > 0.3) game->getWorld()->player->GetCurrentVehicle()->steeringValue = 0.3;
			}
			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_D)) {
				game->getWorld()->player->GetCurrentVehicle()->steeringValue -= 0.04;
				if (game->getWorld()->player->GetCurrentVehicle()->steeringValue < -0.3) game->getWorld()->player->GetCurrentVehicle()->steeringValue = -0.3;
			}
			printf("%f \n", game->getWorld()->player->GetCurrentVehicle()->steeringValue);
		}
		else {
			game->getRenderer()->getCamera().Position = glm::vec3(game->getWorld()->player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), game->getWorld()->player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), game->getWorld()->player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ());

			float speed = game->getInput()->IsKeyPressed(SDL_SCANCODE_LSHIFT) ? 2.0f : 1.0f;

			float length = glm::length(movement);
			if (length > 0.1f) {
				auto move = speed * glm::normalize(movement);
				//move *= delta_time;
				game->getWorld()->player->getPhysCharacter()->setWalkDirection(btVector3(move.x, move.z, 0.f));
			}
			else {
				game->getWorld()->player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
			}

			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_SPACE)) game->getWorld()->player->Jump();
		}
	}

	//MOUSE
	int x;
	int y;
	SDL_GetRelativeMouseState(&x, &y);
	game->getRenderer()->getCamera().ProcessMouseMovement(-x, -y);

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
	}
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT: {
			game->getWorld()->DetectWeaponHit(game->getRenderer()->getCamera().Position, game->getRenderer()->getCamera().Front);
		}
	}
	}
}
