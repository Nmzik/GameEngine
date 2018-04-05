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

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_Q)) {
		game->getRenderer()->hdrEnabled = !game->getRenderer()->hdrEnabled;
	}

	if (game->getInput()->IsKeyPressed(SDL_SCANCODE_E)) {
		//game->getRenderer()->setType(0);
	}

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_Z)) {
		if (game->getWorld()->gameHour > 23) game->getWorld()->gameHour = 0;
		game->getWorld()->gameHour++;
	}

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_X)) {
		if (game->getWorld()->gameHour < 1) game->getWorld()->gameHour = 23;
		game->getWorld()->gameHour--;
	}

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_V)) {
		game->getWorld()->TestFunction(game->getRenderer()->getCamera().Position);
	}
	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_B)) {
		game->getWorld()->currentPlayerID = 0;
		game->getWorld()->player[game->getWorld()->currentPlayerID]->getPhysCharacter()->setGravity(game->getWorld()->GetDynamicsWorld()->getGravity());
		for (int i = 0; i < 3; i++)
		{
			if (game->getWorld()->currentPlayerID != i) {
				game->getWorld()->player[i]->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
			}
		}
	}
	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_N)) {
		game->getWorld()->currentPlayerID = 1;
		game->getWorld()->player[game->getWorld()->currentPlayerID]->getPhysCharacter()->setGravity(game->getWorld()->GetDynamicsWorld()->getGravity());
		for (int i = 0; i < 3; i++)
		{
			if (game->getWorld()->currentPlayerID != i) {
				game->getWorld()->player[i]->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
			}
		}
	}
	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_M)) {
		game->getWorld()->currentPlayerID = 2;
		game->getWorld()->player[game->getWorld()->currentPlayerID]->getPhysCharacter()->setGravity(game->getWorld()->GetDynamicsWorld()->getGravity());
		for (int i = 0; i < 3; i++)
		{
			if (game->getWorld()->currentPlayerID != i) {
				game->getWorld()->player[i]->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
			}
		}
	}
	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_I)) {
		game->getRenderer()->RenderDebugWorld = !game->getRenderer()->RenderDebugWorld;
	}

	Player* player = game->getWorld()->player[game->getWorld()->currentPlayerID];

	if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_F)) {
		DebugPressed = !DebugPressed;
		//if (DebugPressed == false) player->getPhysCharacter()->warp(btVector3(game->getRenderer()->getCamera().Position.x, game->getRenderer()->getCamera().Position.y, game->getRenderer()->getCamera().Position.z));
	}

	if (DebugPressed) {
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_W)) game->getRenderer()->getCamera().ProcessKeyboard(FORWARD, delta_time);
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_S)) game->getRenderer()->getCamera().ProcessKeyboard(BACKWARD, delta_time);
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_A)) game->getRenderer()->getCamera().ProcessKeyboard(LEFT, delta_time);
		if (game->getInput()->IsKeyPressed(SDL_SCANCODE_D)) game->getRenderer()->getCamera().ProcessKeyboard(RIGHT, delta_time);
		//player->getPhysCharacter()->warp(btVector3(game->getRenderer()->getCamera().Position.x, game->getRenderer()->getCamera().Position.y, game->getRenderer()->getCamera().Position.z));
		//player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
	}
	else {

		//NEED PROPER FIX

		if (game->getInput()->IsKeyTriggered(SDL_SCANCODE_U)) {
			if (player->GetCurrentVehicle()) {
				//in Vehicle
				printf("EXITING");
				player->getPhysCharacter()->warp(player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin() + btVector3(0.0f, 50.0f, 0.0f));
				player->ExitVehicle();
			} else {
				printf("ENTERING");
				player->EnterVehicle(game->getWorld()->FindNearestVehicle());
			}
		}

		glm::vec3 movement;
		movement.x = game->getInput()->IsKeyPressed(SDL_SCANCODE_W) - game->getInput()->IsKeyPressed(SDL_SCANCODE_S);
		movement.z = game->getInput()->IsKeyPressed(SDL_SCANCODE_A) - game->getInput()->IsKeyPressed(SDL_SCANCODE_D);

		if (player->GetCurrentVehicle()) {
			game->getRenderer()->getCamera().Position = glm::vec3(player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getX(), player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getY(), player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getZ());
			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_W)) {
				player->GetCurrentVehicle()->SetThrottle(1.0);
			}
			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_S)) {
				player->GetCurrentVehicle()->SetThrottle(-1.0);
			}

			float steering = 0.0f;

			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_A) != game->getInput()->IsKeyPressed(SDL_SCANCODE_D)) {
				steering = (game->getInput()->IsKeyPressed(SDL_SCANCODE_A) ? 0.3 : -0.3);
			}

			player->GetCurrentVehicle()->steeringValue = steering;
			
			//printf("%f \n", player->GetCurrentVehicle()->steeringValue);
		}
		else {
			game->getRenderer()->getCamera().Position = glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ());

			float speed = game->getInput()->IsKeyPressed(SDL_SCANCODE_LSHIFT) ? 2.0f : 1.0f;

			/*bool inWater = false;
			if (game->getWorld()->DetectInWater(glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ())))
			{
				if (player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ() < 0 && !inWater) {
					player->getPhysCharacter()->setGravity(btVector3(0.0f, 0.0f, 0.0f));
					inWater = true;
				}
				else if (inWater == true){
					player->getPhysCharacter()->setGravity(btVector3(0, 0, -10));
					inWater = false;
				}
			}*/

			float length = glm::length(movement);
			if (length > 0.1f) {
				auto move = speed * glm::normalize(movement);
				//move *= delta_time;
				player->getPhysCharacter()->setWalkDirection(btVector3(move.x, move.z, 0.f));
			}
			else {
				player->getPhysCharacter()->setWalkDirection(btVector3(0.f, 0.f, 0.f));
			}

			if (game->getInput()->IsKeyPressed(SDL_SCANCODE_SPACE)) player->Jump();
		}
	}

	/*auto rayEnd = game->getRenderer()->getCamera().Position;
	auto rayStart = glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY() - 5, player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ());
	auto to = btVector3(rayEnd.x, rayEnd.y, rayEnd.z);
	auto from = btVector3(rayStart.x, rayStart.y, rayStart.z);
	btCollisionWorld::ClosestRayResultCallback cb(from, to);

	game->getWorld()->GetDynamicsWorld()->rayTest(from, to, cb);
	if (cb.hasHit() && cb.m_closestHitFraction < 1.f) {
		game->getRenderer()->getCamera().Position =
			glm::vec3(cb.m_hitPointWorld.x(), cb.m_hitPointWorld.y(),
				cb.m_hitPointWorld.z());
		game->getRenderer()->getCamera().Position +=
			glm::vec3(cb.m_hitNormalWorld.x(), cb.m_hitNormalWorld.y(),
				cb.m_hitNormalWorld.z()) *
			0.1f;
	}*/

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
