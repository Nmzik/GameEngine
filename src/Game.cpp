#include "Game.h"

#include "CPed.h"
#include "CVehicle.h"
#include "GameData.h"
#include "GameWorld.h"

#include "InputActions.h"

Game::Game(const char* GamePath)
{
    window = std::make_unique<NativeWindow>();

    gameData = std::make_unique<GameData>(GamePath);
    rendering_system = std::make_unique<RenderingSystem>(window.get());
    gameWorld = std::make_unique<GameWorld>(gameData.get());
    input = std::make_unique<InputManager>();

	camera = std::make_unique<Camera>(glm::vec3(0.0, 0.0, 50.0), gameWorld.get());

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
    
}

void Game::updateFPS(float delta_time, float cpuThreadTime, float gpuThreadTime)
{
    static auto time_since_last_fps_output = 0.f;

    time_since_last_fps_output += delta_time;
    if (time_since_last_fps_output >= 1.0f)
    {
        time_since_last_fps_output = 0.0f;
        std::ostringstream osstr;
        osstr << "Game "
              << " (" << (1.0f / delta_time) << " FPS, " << (delta_time * 1000.0f) << " CPU time, " << (cpuThreadTime * 1000.0f) << " CPU Thread time, "
              << (gpuThreadTime * 1000.0f) << " Render Thread time, " << rendering_system->gpuTime * 0.000001f << " GPU time) | "
              << gameWorld->renderList.size() << " Objects, " << rendering_system->DrawCalls << " Draw Calls, "
              << gameWorld->getResourceManager()->GlobalGpuMemory / 1024 / 1024 << " MB GPU Mem, "
              << gameWorld->getResourceManager()->TextureMemory / 1024 / 1024 << " MB Texture Mem, Bullet Free Mem ";
        //<< gameWorld->objects.size();
        //<< (myAllocator->getSize() - myAllocator->getUsedMemory()) / 1024 / 1024;
        window->setTitle(osstr);
    }
}

void Game::run()
{
    //	SDL_Event event;

    bool running = true;
    auto current_time = std::chrono::steady_clock::now();

    while (running)
    {
        auto cpuThreadStart = std::chrono::steady_clock::now();
        input->Update();

        if (input->IsKeyPressed(Actions::button_ESCAPE))
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
            gameWorld->updateWorld(delta_time, camera.get());
            tick(delta_time);
            camera->onUpdate();
        }
        auto cpuThreadEnd = std::chrono::steady_clock::now();
        float cpuThreadTime = std::chrono::duration<float>(cpuThreadEnd - cpuThreadStart).count();

        auto gpuThreadStart = std::chrono::steady_clock::now();
        rendering_system->render(gameWorld.get(), camera.get());
        auto gpuThreadEnd = std::chrono::steady_clock::now();
        float gpuThreadTime = std::chrono::duration<float>(gpuThreadEnd - gpuThreadStart).count();
        //	SDL_GL_SwapWindow(window);

        updateFPS(delta_time, cpuThreadTime, gpuThreadTime);
    }

    //	if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) break;
}

void Game::tick(float delta_time)
{
    camera->lookcamera = getInput()->getMouseMovement();


    if (getInput()->IsKeyTriggered(Actions::button_E))
    {
        //	getWorld()->LODMultiplier += 0.05f;
        getWorld()->EnableStreaming = !getWorld()->EnableStreaming;
    }

    /*if (getInput()->IsKeyTriggered(Actions::button_Z))
    {
        if (getWorld()->gameHour > 23)
            getWorld()->gameHour = 0;
        getWorld()->gameHour++;
    }

    if (getInput()->IsKeyTriggered(Actions::button_X))
    {
        if (getWorld()->gameHour < 1)
            getWorld()->gameHour = 23;
        getWorld()->gameHour--;
        //	getRenderer()->ShowTexture = !getRenderer()->ShowTexture;
    }*/
    /*if (getInput()->IsKeyTriggered(Actions::button_C))
    {
        getWorld()->TestFunction(getRenderer()->getCamera().position);
    }*/

    //	getWorld()->peds[getWorld()->currentPlayerID].SetPosition(glm::vec3(-205.28, 6432.15, 36.87));
    if (getInput()->IsKeyTriggered(Actions::button_player1))
    {
        getWorld()->currentPlayerID = 0;

        changePlayer();
    }
    if (getInput()->IsKeyTriggered(Actions::button_player2))
    {
        getWorld()->currentPlayerID = 1;

        changePlayer();
    }
    if (getInput()->IsKeyTriggered(Actions::button_player3))
    {
        getWorld()->currentPlayerID = 2;

        changePlayer();
    }
    if (getInput()->IsKeyTriggered(Actions::button_ShowCollision))
    {
        getRenderer()->RenderDebugWorld = !getRenderer()->RenderDebugWorld;
    }

    CPed* player = getWorld()->peds[getWorld()->currentPlayerID];

    //	printf("CURRENT HEALTH %f\n", player->getCurrentHealth());

    if (getInput()->IsKeyTriggered(Actions::button_CameraMode))
    {
        if (camera->getCameraMode() != CameraMode::FreeCamera)
            camera->setCameraMode(CameraMode::FreeCamera);
        else
            camera->setCameraMode(CameraMode::ThirdPerson);
    }

    //	NEED PROPER FIX

    if (getInput()->IsKeyTriggered(Actions::button_EnterExitVehicle))
    {
        if (player->getCurrentVehicle())
        {
            //	in Vehicle
            printf("EXITING");
            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(player->getCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin() + btVector3(0.0f, 0.0f, 3.0f));
            player->getPhysCharacter()->setWorldTransform(transform);

            player->exitVehicle();
            player->getPhysCharacter()->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter;

            player->getPhysCharacter()->setGravity(PhysicsSystem::dynamicsWorld->getGravity());
        }
        else
        {
            printf("ENTERING");
            player->enterVehicle(getWorld()->findNearestVehicle());
            if (player->getCurrentVehicle())
            {
                player->getPhysCharacter()->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::DefaultFilter;
                player->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
            }
        }
    }

    glm::vec3 movement{};
    movement.x = (float)getInput()->IsKeyPressed(Actions::button_Forward) - getInput()->IsKeyPressed(Actions::button_Backward);
    movement.y = (float)getInput()->IsKeyPressed(Actions::button_TurnLeft) - getInput()->IsKeyPressed(Actions::button_TurnRight);

	movement *= 5;

    if (camera->getCameraMode() == CameraMode::FreeCamera)
    {
        glm::vec3 camPos = camera->getPosition();
        if (getInput()->IsKeyPressed(Actions::button_CameraUp))
        {
            camPos.z += 10;
        }

        if (getInput()->IsKeyPressed(Actions::button_CameraDown))
        {
            camPos.z -= 10;
        }

        camera->setPosition(camPos + movement);
    }

    else if (player->getCurrentVehicle())
    {
        if (player->getCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin().getZ() <= -150)
        {
            player->getCurrentVehicle()->m_carChassis->setWorldTransform(
                btTransform(btQuaternion(0, 0, 0, 1), player->getCurrentVehicle()->m_carChassis->getWorldTransform().getOrigin() + btVector3(0, 0, 300)));
        }

        if (getInput()->IsKeyPressed(Actions::button_Forward))
        {
            player->getCurrentVehicle()->setThrottle(1.0);
        }
        else if (getInput()->IsKeyPressed(Actions::button_Backward))
        {
            player->getCurrentVehicle()->setThrottle(-1.0);
        }
        else
        {
            player->getCurrentVehicle()->setThrottle(0.0);
        }

        float steering = getInput()->IsKeyPressed(Actions::button_TurnLeft) ? 0.3f : getInput()->IsKeyPressed(Actions::button_TurnRight) ? -0.3f : 0.0;

        player->getCurrentVehicle()->setSteeringValue(steering);

        player->getPhysCharacter()->setWorldTransform(player->getCurrentVehicle()->m_carChassis->getWorldTransform());
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

        float speed = getInput()->IsKeyPressed(Actions::button_LSHIFT) ? 30.0f : 1.0f;

        /*bool inWater = false;
			if (getWorld()->DetectInWater(glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(),
			player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY(),
			player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ())))
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
        player->getPhysCharacter()->setLinearVelocity(
            btVector3(movement.x * 10.0f * speed, movement.y * 10.0f * speed, player->getPhysCharacter()->getLinearVelocity().z()));

        if (getInput()->IsKeyTriggered(Actions::button_SPACE))
        {
            //	player->Jump();

            btVector3 m_rayStart = player->getPhysCharacter()->getCenterOfMassPosition();
            btVector3 m_rayEnd = m_rayStart - btVector3(0.0, 0.0, 1.5);

            // rayCallback
            btCollisionWorld::ClosestRayResultCallback rayCallback(m_rayStart, m_rayEnd);

            PhysicsSystem::dynamicsWorld->rayTest(m_rayStart, m_rayEnd, rayCallback);
            if (rayCallback.hasHit())
            {  //	JUMP!
                player->getPhysCharacter()->applyCentralImpulse(btVector3(0.f, 0.f, 50.0f));
            }
        }
    }
    /*auto rayEnd = getRenderer()->getCamera().Position;
	auto rayStart = glm::vec3(player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getX(),
	player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getY() - 5,
	player->getPhysCharacter()->getGhostObject()->getWorldTransform().getOrigin().getZ()); auto to = btVector3(rayEnd.x, rayEnd.y, rayEnd.z); auto from =
	btVector3(rayStart.x, rayStart.y, rayStart.z); btCollisionWorld::ClosestRayResultCallback cb(from, to);

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

void Game::changePlayer()
{
    uint32_t random = rand() % getWorld()->getGameData()->Scenes.size();
    getWorld()->peds[getWorld()->currentPlayerID]->setPosition(glm::vec3(-205.28, 6432.15, 36.87));
    getWorld()->peds[getWorld()->currentPlayerID]->getPhysCharacter()->setGravity(PhysicsSystem::dynamicsWorld->getGravity());
    for (int i = 0; i < 3; i++)
    {
        if (getWorld()->currentPlayerID != i)
        {
            getWorld()->peds[i]->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
        }
    }
}
