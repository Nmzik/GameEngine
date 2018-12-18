#include "Game.h"

#include "GameWorld.h"
#include "RenderingSystem.h"
#include "GameData.h"
#include "CPed.h"

Game::Game()
{

	SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

	window = SDL_CreateWindow("SDL2 window", // window title
		SDL_WINDOWPOS_UNDEFINED,             // initial x position
		SDL_WINDOWPOS_UNDEFINED,             // initial y position
		1280,                                // width, in pixels
		720,                                 // height, in pixels
		SDL_WINDOW_OPENGL                    //| SDL_WINDOW_FULLSCREEN                 // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL)
	{
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);

	rendering_system = std::make_unique<RenderingSystem>(window);
	gameWorld = std::make_unique<GameWorld>();
	input = std::make_unique<InputManager>();

	StateManager::get().enter<InGameState>(this);

	//	FT_Library library;
	//	FT_Init_FreeType(&library);

	// get format from audio file

	//	Libavcodec
	//	FFMPEG STUFF ADPCM OR PCM | it seems that all audio files are pcm!
	/*AVFormatContext* format = avformat_alloc_context();
	if (avformat_open_input(&format, "C:/Users/nmzik/Desktop/0x13B709E8.wav", NULL, NULL) != 0) {
	 printf("Could not open file");
	 return;
	}
	if (avformat_find_stream_info(format, NULL) < 0) {
	 printf("Could not retrieve stream info from file");
	 return;
	}

	// Find the index of the first audio stream
	int stream_index = -1;
	for (int i = 0; i<format->nb_streams; i++) {
	 if (format->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
	  stream_index = i;
	  break;
	 }
	}
	if (stream_index == -1) {
	 printf("Could not retrieve audio stream from file");
	 return;
	}

	AVStream* stream = format->streams[stream_index];

	// find & open codec
	AVCodecContext* codec = stream->codec;
	if (avcodec_open2(codec, avcodec_find_decoder(codec->codec_id), NULL) < 0) {
	 printf("Failed to open decoder for stream in file");
	 return;
	}

	AVPacket packet;
	av_init_packet(&packet);
	*/
}

Game::~Game()
{
	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::updateFPS(float delta_time)
{
	static auto time_since_last_fps_output = 0.f;

	time_since_last_fps_output += delta_time;
	if (time_since_last_fps_output >= 1.0f)
	{
		time_since_last_fps_output = 0.0f;
		std::ostringstream osstr;
		osstr << "Game window"
			<< " (" << (1.0f / delta_time) << " FPS, " << (delta_time * 1000.0f) << " CPU time, " << rendering_system->gpuTime * 0.000001f << " GPU time) | " << gameWorld->renderList.size()
			<< " Objects, " << rendering_system->DrawCalls << " Draw Calls, " << gameWorld->GetResourceManager()->GlobalGpuMemory / 1024 / 1024 << " MB GPU memory, "
			<< gameWorld->GetResourceManager()->TextureMemory / 1024 / 1024 << " MB Texture Memory";
		SDL_SetWindowTitle(window, osstr.str().c_str());
	}
}

void Game::run()
{

	//	SDL_Event event;

	bool running = true;
	auto current_time = std::chrono::steady_clock::now();

	while (running)
	{
		input->Update();

		if (input->IsKeyPressed(SDL_SCANCODE_ESCAPE))
			running = false;
		/*while (SDL_PollEvent(&event)) {
		 StateManager::get().currentState()->handleEvent(event);
		 //if (event.type == SDL_QUIT) running = false;
		}*/
		auto old_time = current_time;
		current_time = std::chrono::steady_clock::now();
		float delta_time = std::chrono::duration<float>(current_time - old_time).count();

		if (!paused)
		{
			gameWorld->update(delta_time, &rendering_system->getCamera());
			tick(delta_time);
		}

		rendering_system->render(gameWorld.get());
		//SDL_GL_SwapWindow(window);

		updateFPS(delta_time);
	}

	//	if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) break;
}

void Game::tick(float delta_time)
{
	static float clockAccumulator = 0.f;

	clockAccumulator += delta_time;
	while (clockAccumulator >= 1.f)
	{
		getWorld()->gameMinute++;
		if (getWorld()->gameMinute >= 60)
		{
			getWorld()->gameMinute = 0;
			getWorld()->gameHour++;
			if (getWorld()->gameHour >= 24)
			{
				getWorld()->gameHour = 0;
			}
		}
		clockAccumulator -= 1.f;
	}

	//	printf("Time %d %d\n", getWorld()->gameHour, getWorld()->gameMinute);

	static bool DebugPressed = true;
	//	KEYBOARD

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_Q))
	{
		//	getWorld()->LODMultiplier -= 0.05f;
		getRenderer()->gpuTimer = !getRenderer()->gpuTimer;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_E))
	{
		//	getWorld()->LODMultiplier += 0.05f;
		getWorld()->EnableStreaming = !getWorld()->EnableStreaming;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_Z))
	{
		if (getWorld()->gameHour > 23)
			getWorld()->gameHour = 0;
		getWorld()->gameHour++;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_X))
	{
		if (getWorld()->gameHour < 1)
			getWorld()->gameHour = 23;
		getWorld()->gameHour--;
		//	getRenderer()->ShowTexture = !getRenderer()->ShowTexture;
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_V))
	{
		getWorld()->TestFunction(getRenderer()->getCamera().position);
	}

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_C))
	{
		getWorld()->ClearTestFunction();
	}

	//getWorld()->peds[getWorld()->currentPlayerID].SetPosition(glm::vec3(-205.28, 6432.15, 36.87));
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_B))
	{
		getWorld()->currentPlayerID = 0;

		ChangePlayer();
	}
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_N))
	{
		getWorld()->currentPlayerID = 1;

		ChangePlayer();
	}
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_M))
	{
		getWorld()->currentPlayerID = 2;

		ChangePlayer();
	}
	if (getInput()->IsKeyTriggered(SDL_SCANCODE_I))
	{
		getRenderer()->RenderDebugWorld = !getRenderer()->RenderDebugWorld;
	}

	CPed* player = &getWorld()->peds[getWorld()->currentPlayerID];

	//	printf("CURRENT HEALTH %f\n", player->getCurrentHealth());

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_F))
	{
		DebugPressed = !DebugPressed;
	}

	//	NEED PROPER FIX

	if (getInput()->IsKeyTriggered(SDL_SCANCODE_U))
	{
		if (player->GetCurrentVehicle())
		{
			//	in Vehicle
			printf("EXITING");
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin() + btVector3(0.0f, 0.0f, 3.0f));
			player->getPhysCharacter()->setWorldTransform(transform);

			player->ExitVehicle();
			player->getPhysCharacter()->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter;

			player->getPhysCharacter()->setGravity(PhysicsSystem::dynamicsWorld->getGravity());
		}
		else
		{
			printf("ENTERING");
			player->EnterVehicle(getWorld()->FindNearestVehicle());
			if (player->GetCurrentVehicle())
			{
				player->getPhysCharacter()->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::DefaultFilter;
				player->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
			}
		}
	}

	glm::vec3 movement{};
	movement.x = (float)getInput()->IsKeyPressed(SDL_SCANCODE_W) - getInput()->IsKeyPressed(SDL_SCANCODE_S);
	movement.y = (float)getInput()->IsKeyPressed(SDL_SCANCODE_A) - getInput()->IsKeyPressed(SDL_SCANCODE_D);

	if (player->GetCurrentVehicle())
	{

		if (player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getZ() <= -150)
		{
			player->GetCurrentVehicle()->m_carChassis->setWorldTransform(
				btTransform(btQuaternion(0, 0, 0, 1), player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin() + btVector3(0, 0, 300)));
		}

		getRenderer()->getCamera().position = glm::vec3(player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getX(),
			player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getY() - 5.f, player->GetCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getZ() + 5.0f);
		if (getInput()->IsKeyPressed(SDL_SCANCODE_W))
		{
			player->GetCurrentVehicle()->SetThrottle(1.0);
		}
		else if (getInput()->IsKeyPressed(SDL_SCANCODE_S))
		{
			player->GetCurrentVehicle()->SetThrottle(-1.0);
		}
		else
		{
			player->GetCurrentVehicle()->SetThrottle(0.0);
		}

		float steering = 0.0f;

		if (getInput()->IsKeyPressed(SDL_SCANCODE_A) != getInput()->IsKeyPressed(SDL_SCANCODE_D))
		{
			steering = (getInput()->IsKeyPressed(SDL_SCANCODE_A) ? 0.3f : -0.3f);
		}

		player->GetCurrentVehicle()->steeringValue = steering;

		player->getPhysCharacter()->setWorldTransform(player->GetCurrentVehicle()->m_carChassis->getWorldTransform());
	}
	else
	{

		if (player->position.z <= -50)
		{

			btVector3 rayFrom(player->position.x, player->position.y, 300.f);
			btVector3 rayTo(player->position.x, player->position.y, 0.f);

			btDynamicsWorld::ClosestRayResultCallback rr(rayFrom, rayTo);

			PhysicsSystem::dynamicsWorld->rayTest(rayFrom, rayTo, rr);

			if (rr.hasHit())
			{
				auto& ws = rr.m_hitPointWorld;
				player->getPhysCharacter()->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(ws.x(), ws.y(), ws.z() + 10.0f)));
			}
		}

		/*if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		 getRenderer()->getCamera().Position = glm::vec3(player->position.getX(), player->position.getY() - 5.0f, player->position.getZ());
		else
		 getRenderer()->getCamera().Position = glm::vec3(player->position.getX(), player->position.getY(), player->position.getZ());
*/
		if (DebugPressed)
		{
			getRenderer()->getCamera().position += movement;
		}
		else
		{

			float speed = getInput()->IsKeyPressed(SDL_SCANCODE_LSHIFT) ? 2.0f : 1.0f;

			/*bool inWater = false;
			if (getWorld()->DetectInWater(glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(),
			player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ())))
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

			/*float length = glm::length(movement);
			if (length > 0.1f) {
			 auto move = speed * glm::normalize(movement);
			 //move *= delta_time;*/

			 /*if (player->getPhysCharacter()->getLinearVelocity().z() < -40.f) {
			  player->getPhysCharacter()->setLinearVelocity(btVector3(movement.x * 30.0f, movement.y * 30.0f, -40.0f));
			 }
			 else*/
			player->getPhysCharacter()->setLinearVelocity(btVector3(movement.x * 30.0f, movement.y * 30.0f, player->getPhysCharacter()->getLinearVelocity().z()));

			if (getInput()->IsKeyTriggered(SDL_SCANCODE_SPACE))
			{
				//	player->Jump();

				btVector3 m_rayStart = player->getPhysCharacter()->getCenterOfMassPosition();
				btVector3 m_rayEnd = m_rayStart - btVector3(0.0, 0.0, 1.5);

				// rayCallback
				btCollisionWorld::ClosestRayResultCallback rayCallback(m_rayStart, m_rayEnd);

				PhysicsSystem::dynamicsWorld->rayTest(m_rayStart, m_rayEnd, rayCallback);
				if (rayCallback.hasHit())
				{ //	JUMP!
					player->getPhysCharacter()->applyCentralImpulse(btVector3(0.f, 0.f, 50.0f));
				}
			}
		}
	}


	//	MOUSE
	int x;
	int y;
	SDL_GetMouseState(&x, &y);

	glm::vec3 targetPosition = glm::vec3(player->position.x, player->position.y, player->position.z);

	auto look = glm::vec2(x * 0.01f, y * 0.01f);
	// Determine the "ideal" camera position for the current view angles
	auto yaw = glm::angleAxis(-look.x - glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f));
	auto pitch = glm::angleAxis(look.y, glm::vec3(0.f, 1.f, 0.f));
	auto cameraOffset = yaw * pitch * glm::vec3(0.f, 0.f, 5.0f);
	glm::vec3 cameraPosition = targetPosition + cameraOffset;

	glm::vec3 lookTargetPosition(targetPosition);
	targetPosition += glm::vec3(0.f, 0.f, 1.f);
	lookTargetPosition += glm::vec3(0.f, 0.f, 0.5f);

	auto lookdir = glm::normalize(lookTargetPosition - cameraPosition);
	// Calculate the angles to look at the target position
	float len2d = glm::length(glm::vec2(lookdir));
	float anglePitch = glm::atan(lookdir.z, len2d);
	float angleYaw = glm::atan(lookdir.y, lookdir.x);
	glm::quat angle(glm::vec3(0.f, -anglePitch, angleYaw));

	getRenderer()->getCamera().position = cameraPosition;
	getRenderer()->getCamera().rotation = angle;

	/*auto rayEnd = getRenderer()->getCamera().Position;
	auto rayStart = glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(), player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY() -
	5, player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ()); auto to = btVector3(rayEnd.x, rayEnd.y, rayEnd.z); auto from = btVector3(rayStart.x, rayStart.y, rayStart.z);
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

	//	getRenderer()->getCamera().ProcessMouseMovement(-x, -y);
}

void Game::ChangePlayer()
{
	uint32_t random = rand() % getWorld()->getGameData()->Scenes.size();
	getWorld()->peds[getWorld()->currentPlayerID].SetPosition(glm::vec3(-205.28, 6432.15, 36.87));
	getWorld()->peds[getWorld()->currentPlayerID].getPhysCharacter()->setGravity(PhysicsSystem::dynamicsWorld->getGravity());
	for (int i = 0; i < 3; i++)
	{
		if (getWorld()->currentPlayerID != i)
		{
			getWorld()->peds[i].getPhysCharacter()->setGravity(btVector3(0, 0, 0));
		}
	}
}
