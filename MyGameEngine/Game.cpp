#include "Game.h"

Game::Game() {

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	window = SDL_CreateWindow(
		"SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		1280,                               // width, in pixels
		720,                               // height, in pixels
		SDL_WINDOW_OPENGL  //| SDL_WINDOW_FULLSCREEN                 // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);

	rendering_system = std::make_unique<RenderingSystem>(window);
	gameWorld = std::make_unique<GameWorld>();
	input = new InputManager();

	StateManager::get().enter<InGameState>(this);

	FT_Library library;
	FT_Init_FreeType(&library);

}


Game::~Game() {
	// Close and destroy the window
	//SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::updateFPS(float delta_time) {
	static auto time_since_last_fps_output = 0.f;

	time_since_last_fps_output += delta_time;
	if (time_since_last_fps_output >= 1.0f) {
		time_since_last_fps_output = 0.0f;
		std::ostringstream osstr;
		osstr << "SDL2 window" << " (" << (1.0f / delta_time) << " FPS, CPU time " << (delta_time * 1000.0f) << ") | GPU time " << rendering_system->gpuTime * 0.000001f << "ms " << gameWorld->renderList.size() << "Objects " << rendering_system->DrawCalls << "Draw Calls";
		SDL_SetWindowTitle(window, osstr.str().c_str());
	}
}

void Game::run() {

	//SDL_Event event;

	bool running = true;
	auto current_time = std::chrono::steady_clock::now();
	
	while (running) {
		input->Update();

		if (input->IsKeyPressed(SDL_SCANCODE_ESCAPE))
			running = false;
		/*while (SDL_PollEvent(&event)) {
			StateManager::get().currentState()->handleEvent(event);
			//if (event.type == SDL_QUIT) running = false;
		}*/

		if (!paused) {
			auto old_time = current_time;
			current_time = std::chrono::steady_clock::now();
			float delta_time = std::chrono::duration<float>(current_time - old_time).count();

			gameWorld->update(delta_time, &rendering_system->getCamera());
			tick(delta_time);
			rendering_system->render(gameWorld.get());
			SDL_GL_SwapWindow(window);

			updateFPS(delta_time);
		}
	}

	//if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) break;
}

void Game::tick(float delta_time)
{
	static float clockAccumulator = 0.f;

	clockAccumulator += delta_time;
	while (clockAccumulator >= 1.f) {
		getWorld()->gameMinute++;
		if (getWorld()->gameMinute >= 60) {
			getWorld()->gameMinute = 0;
			getWorld()->gameHour++;
			if (getWorld()->gameHour >= 24) {
				getWorld()->gameHour = 0;
			}
		}
		clockAccumulator -= 1.f;
	}

	//printf("Time %d %d\n", getWorld()->gameHour, getWorld()->gameMinute);

	static bool DebugPressed = true;
	//KEYBOARD

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_Q)) {
		//getWorld()->LODMultiplier -= 0.05f;
		getRenderer()->gpuTimer = !getRenderer()->gpuTimer;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_E)) {
		//getWorld()->LODMultiplier += 0.05f;
		getWorld()->EnableStreaming = !getWorld()->EnableStreaming;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_Z)) {
		if (getWorld()->gameHour > 23) getWorld()->gameHour = 0;
		getWorld()->gameHour++;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_X)) {
		if (getWorld()->gameHour < 1) getWorld()->gameHour = 23;
		getWorld()->gameHour--;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_V)) {
		getWorld()->TestFunction(getRenderer()->getCamera().Position);
	}
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_B)) {
		getWorld()->currentPlayerID = 0;
		uint32_t random = rand() % getWorld()->getGameData()->Scenes.size();
		getWorld()->pedestrians[getWorld()->currentPlayerID].SetPosition(getWorld()->getGameData()->Scenes[random]);
		getWorld()->pedestrians[getWorld()->currentPlayerID].getPhysCharacter()->setGravity(getWorld()->GetDynamicsWorld()->getGravity());
		for (int i = 0; i < 3; i++)
		{
			if (getWorld()->currentPlayerID != i) {
				getWorld()->pedestrians[i].getPhysCharacter()->setGravity(btVector3(0, 0, 0));
			}
		}
	}
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_N)) {
		getWorld()->currentPlayerID = 1;
		uint32_t random = rand() % getWorld()->getGameData()->Scenes.size();
		getWorld()->pedestrians[getWorld()->currentPlayerID].SetPosition(getWorld()->getGameData()->Scenes[random]);
		getWorld()->pedestrians[getWorld()->currentPlayerID].getPhysCharacter()->setGravity(getWorld()->GetDynamicsWorld()->getGravity());
		for (int i = 0; i < 3; i++)
		{
			if (getWorld()->currentPlayerID != i) {
				getWorld()->pedestrians[i].getPhysCharacter()->setGravity(btVector3(0, 0, 0));
			}
		}
	}
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_M)) {
		getWorld()->currentPlayerID = 2;
		uint32_t random = rand() % getWorld()->getGameData()->Scenes.size();
		getWorld()->pedestrians[getWorld()->currentPlayerID].SetPosition(getWorld()->getGameData()->Scenes[random]);
		getWorld()->pedestrians[getWorld()->currentPlayerID].getPhysCharacter()->setGravity(getWorld()->GetDynamicsWorld()->getGravity());
		for (int i = 0; i < 3; i++)
		{
			if (getWorld()->currentPlayerID != i) {
				getWorld()->pedestrians[i].getPhysCharacter()->setGravity(btVector3(0, 0, 0));
			}
		}
	}
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_I)) {
		getRenderer()->RenderDebugWorld = !getRenderer()->RenderDebugWorld;
	}

	Player* player = &getWorld()->pedestrians[getWorld()->currentPlayerID];

	//printf("CURRENT HEALTH %f\n", player->getCurrentHealth());

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_F)) {
		DebugPressed = !DebugPressed;
	}

	if (DebugPressed) {
		if (getInput()->IsKeyPressed(SDL_SCANCODE_W)) getRenderer()->getCamera().ProcessKeyboard(FORWARD, delta_time);
		if (getInput()->IsKeyPressed(SDL_SCANCODE_S)) getRenderer()->getCamera().ProcessKeyboard(BACKWARD, delta_time);
		if (getInput()->IsKeyPressed(SDL_SCANCODE_A)) getRenderer()->getCamera().ProcessKeyboard(LEFT, delta_time);
		if (getInput()->IsKeyPressed(SDL_SCANCODE_D)) getRenderer()->getCamera().ProcessKeyboard(RIGHT, delta_time);
		//player->getPhysCharacter()->warp(btVector3(getRenderer()->getCamera().Position.x, getRenderer()->getCamera().Position.y, getRenderer()->getCamera().Position.z));
	}
	else {

		//NEED PROPER FIX

		if (getInput()->IsKeyTriggered(SDL_SCANCODE_U)) {
			if (player->GetCurrentVehicle()) {
				//in Vehicle
				printf("EXITING");
				btTransform transform;
				transform.setIdentity();
				transform.setOrigin(player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin() + btVector3(0.0f, 0.0f, 3.0f));
				player->getPhysCharacter()->setWorldTransform(transform);

				player->ExitVehicle();
				player->getPhysCharacter()->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter;

				player->getPhysCharacter()->setGravity(getWorld()->GetDynamicsWorld()->getGravity());
			}
			else {
				printf("ENTERING");
				player->EnterVehicle(getWorld()->FindNearestVehicle());
				if (player->GetCurrentVehicle()) {
					player->getPhysCharacter()->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::DefaultFilter;
					player->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
				}
			}
		}

		glm::vec3 movement;
		movement.x = getInput()->IsKeyPressed(SDL_SCANCODE_W) - getInput()->IsKeyPressed(SDL_SCANCODE_S);
		movement.z = getInput()->IsKeyPressed(SDL_SCANCODE_A) - getInput()->IsKeyPressed(SDL_SCANCODE_D);

		if (player->GetCurrentVehicle()) {

			if (player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getZ() <= -150) {
				player->GetCurrentVehicle()->m_carChassis->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin() + btVector3(0, 0, 300)));
			}

			getRenderer()->getCamera().Position = glm::vec3(player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getX(), player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getY(), player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getZ());
			if (getInput()->IsKeyPressed(SDL_SCANCODE_W)) {
				player->GetCurrentVehicle()->SetThrottle(1.0);
			}
			else if (getInput()->IsKeyPressed(SDL_SCANCODE_S)) {
				player->GetCurrentVehicle()->SetThrottle(-1.0);
			}
			else {
				player->GetCurrentVehicle()->SetThrottle(0.0);
			}

			float steering = 0.0f;

			if (getInput()->IsKeyPressed(SDL_SCANCODE_A) != getInput()->IsKeyPressed(SDL_SCANCODE_D)) {
				steering = (getInput()->IsKeyPressed(SDL_SCANCODE_A) ? 0.3f : -0.3f);
			}

			player->GetCurrentVehicle()->steeringValue = steering;

			player->getPhysCharacter()->setWorldTransform(player->GetCurrentVehicle()->m_carChassis->getWorldTransform());
		}
		else {

			if (player->getPhysCharacter()->getWorldTransform().getOrigin().getZ() <= -150) {
				player->getPhysCharacter()->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), player->getPhysCharacter()->getWorldTransform().getOrigin() + btVector3(0, 0, 300)));
			}

			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
				getRenderer()->getCamera().Position = glm::vec3(player->getPhysCharacter()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getWorldTransform().getOrigin().getY() - 5.0f, player->getPhysCharacter()->getWorldTransform().getOrigin().getZ());
			else
				getRenderer()->getCamera().Position = glm::vec3(player->getPhysCharacter()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getWorldTransform().getOrigin().getY(), player->getPhysCharacter()->getWorldTransform().getOrigin().getZ());

			float speed = getInput()->IsKeyPressed(SDL_SCANCODE_LSHIFT) ? 2.0f : 1.0f;

			/*bool inWater = false;
			if (getWorld()->DetectInWater(glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ())))
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
				player->getPhysCharacter()->setLinearVelocity(btVector3(move.x * 30, move.z * 30, player->getPhysCharacter()->getLinearVelocity().z()));
			}
			else {
				player->getPhysCharacter()->setLinearVelocity(btVector3(0.f, 0.f, player->getPhysCharacter()->getLinearVelocity().z()));
			}

			if (getInput()->IsKeyTriggered(SDL_SCANCODE_SPACE)) player->Jump();
		}
	}

	/*auto rayEnd = getRenderer()->getCamera().Position;
	auto rayStart = glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY() - 5, player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ());
	auto to = btVector3(rayEnd.x, rayEnd.y, rayEnd.z);
	auto from = btVector3(rayStart.x, rayStart.y, rayStart.z);
	btCollisionWorld::ClosestRayResultCallback cb(from, to);

	getWorld()->GetDynamicsWorld()->rayTest(from, to, cb);
	if (cb.hasHit() && cb.m_closestHitFraction < 1.f) {
	getRenderer()->getCamera().Position =
	glm::vec3(cb.m_hitPointWorld.x(), cb.m_hitPointWorld.y(),
	cb.m_hitPointWorld.z());
	getRenderer()->getCamera().Position +=
	glm::vec3(cb.m_hitNormalWorld.x(), cb.m_hitNormalWorld.y(),
	cb.m_hitNormalWorld.z()) *
	0.1f;
	}*/

	//MOUSE
	int x;
	int y;
	SDL_GetRelativeMouseState(&x, &y);
	getRenderer()->getCamera().ProcessMouseMovement(-x, -y);

}