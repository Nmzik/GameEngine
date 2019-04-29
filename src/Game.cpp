﻿#include "Game.h"

#include "CPed.h"
#include "CVehicle.h"
#include "Camera.h"
#include "GTAEncryption.h"
#include "GameData.h"
#include "GameRenderer.h"
#include "GameWorld.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "ScriptInterpreter.h"

#include "InputActions.h"

extern FreeListAllocator* physicsAllocator;

Game::Game(const char* GamePath)
    : paused(false)
    , gameTime(0)
{
    gameData = std::make_unique<GameData>(GamePath);
    window = std::make_unique<Win32Window>();
    rendering_system = std::make_unique<GameRenderer>(window.get());
    gameWorld = std::make_unique<GameWorld>(gameData.get());
    input = std::make_unique<InputManager>();
    scriptMachine = std::make_unique<ScriptInterpreter>(gameData.get(), this);

    //scriptMachine->startThread(GenHash("startup_install"));
    window->AddListener(input.get());

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

        static std::ostringstream osstr;
        osstr.str("");
        osstr.clear();

        osstr << "Game "
              << (1.0f / delta_time) << " FPS, "
              << (delta_time * 1000.0f) << " CPU time, "
              << (cpuThreadTime * 1000.0f) << " CPU Thread time, "
              << (gpuThreadTime * 1000.0f) << " Render Thread time, "
              //<< rendering_system->gpuTime * 0.000001f << " GPU time, "
              << gameWorld->culledYmaps << " Culled ymaps, "
              << gameWorld->renderList.size() << " Objects, "
              << rendering_system->getNumDrawCalls() << " Draw Calls, "
              << gameWorld->getResourceManager()->GlobalGpuMemory / 1024 / 1024 << " MB GPU Mem, "
              << gameWorld->getResourceManager()->TextureMemory / 1024 / 1024 << " MB Texture Mem, "
              << (physicsAllocator->getSize() - physicsAllocator->getUsedMemory()) / 1024 / 1024 << " MB Bullet Free Mem ";
        window->setTitle(osstr.str());
    }
}

void Game::run()
{
    bool running = true;
    auto current_time = std::chrono::steady_clock::now();

    while (running)
    {
        auto cpuThreadStart = std::chrono::steady_clock::now();
        //input->Update();
        window->ProcessEvents();

        if (input->IsKeyPressed(Actions::button_ESCAPE))
            break;
        //running = false;

        auto old_time = current_time;
        current_time = std::chrono::steady_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - old_time).count();
        gameTime += delta_time;

        if (!paused)
        {
            scriptMachine->execute();
            gameWorld->updateWorld(delta_time, camera.get());
            tick(delta_time);
            camera->onUpdate(gameWorld->getCurrentPlayer());
        }
        auto cpuThreadEnd = std::chrono::steady_clock::now();
        float cpuThreadTime = std::chrono::duration<float>(cpuThreadEnd - cpuThreadStart).count();

        auto gpuThreadStart = std::chrono::steady_clock::now();
        rendering_system->renderWorld(gameWorld.get(), camera.get());
        auto gpuThreadEnd = std::chrono::steady_clock::now();
        float gpuThreadTime = std::chrono::duration<float>(gpuThreadEnd - gpuThreadStart).count();

        updateFPS(delta_time, cpuThreadTime, gpuThreadTime);
    }
}

void Game::tick(float delta_time)
{
    camera->lookcamera = getInput()->getMouseMovement();

    if (getInput()->IsKeyTriggered(Actions::button_E))
    {
        camera->setPosition(glm::vec3(0.f));
        //	getWorld()->LODMultiplier += 0.05f;
        //getWorld()->EnableStreaming = !getWorld()->EnableStreaming;
    }

    /*if (getInput()->IsKeyTriggered(Actions::button_GPU_TIME))
    {
        getRenderer()->gpuTimer = !getRenderer()->gpuTimer;
        //clear variable
        getRenderer()->gpuTime = 0;
    }*/

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

    if (getInput()->IsKeyTriggered(Actions::button_player1))
    {
        getWorld()->currentPlayerID = 0;

        changeLocation();
    }
    if (getInput()->IsKeyTriggered(Actions::button_player2))
    {
        getWorld()->currentPlayerID = 1;

        //changePlayer();
    }
    if (getInput()->IsKeyTriggered(Actions::button_player3))
    {
        getWorld()->currentPlayerID = 2;

        //changePlayer();
    }
    /*if (getInput()->IsKeyTriggered(Actions::button_ShowCollision))
    {
        getRenderer()->RenderDebugWorld = !getRenderer()->RenderDebugWorld;
    }*/

    CPed* player = getWorld()->getCurrentPlayer();

    //printf("CUR POS PLAYER %s\n", glm::to_string(player->getPosition()).c_str());

    if (getInput()->IsKeyTriggered(Actions::button_CameraMode))
    {
        if (camera->getCameraMode() != CameraMode::FreeCamera)
        {
            getWorld()->getPhysicsSystem()->putRigidBodyToSleep(player->getPhysCharacter());
            camera->setCameraMode(CameraMode::FreeCamera);
        }
        else
        {
            getWorld()->getPhysicsSystem()->wakeRigidBodyFromSleep(player->getPhysCharacter());
            camera->setCameraMode(CameraMode::ThirdPerson);
        }
    }

    if (getInput()->IsKeyTriggered(Actions::button_EnterExitVehicle))
    {
        if (player->getCurrentVehicle())
        {
            //	in Vehicle
            printf("EXITING\n");
            player->setPosition(player->getCurrentVehicle()->getPosition());

            player->exitVehicle();
            getWorld()->getPhysicsSystem()->wakeRigidBodyFromSleep(player->getPhysCharacter());
        }
        else
        {
            printf("ENTERING\n");
            player->enterVehicle(getWorld()->findNearestVehicle());
            if (player->getCurrentVehicle())
            {
                getWorld()->getPhysicsSystem()->putRigidBodyToSleep(player->getPhysCharacter());
            }
        }
    }

    float speed = getInput()->IsKeyPressed(Actions::button_LSHIFT) ? 30.0f : 1.0f;

    glm::vec3 movement{};
    movement.x = (float)getInput()->IsKeyPressed(Actions::button_Forward) - getInput()->IsKeyPressed(Actions::button_Backward);
    movement.y = (float)getInput()->IsKeyPressed(Actions::button_TurnLeft) - getInput()->IsKeyPressed(Actions::button_TurnRight);

    movement *= speed;

    if (camera->getCameraMode() == CameraMode::FreeCamera)
    {
        glm::vec3 camPos = camera->getPosition();
        if (getInput()->IsKeyPressed(Actions::button_CameraUp))
        {
            camPos.z += 5;
        }

        if (getInput()->IsKeyPressed(Actions::button_CameraDown))
        {
            camPos.z -= 5;
        }

        camera->setPosition(camPos + movement);
    }
    else
    {
        if (player->getCurrentVehicle())
        {
            CVehicle* curVehicle = player->getCurrentVehicle();

            glm::vec3 vehiclePosition = curVehicle->getPosition();

            if (vehiclePosition.z <= -150)
            {
                curVehicle->setPosition(glm::vec3(vehiclePosition.x, vehiclePosition.y, vehiclePosition.z + 300.f));
            }

            if (getInput()->IsKeyPressed(Actions::button_Forward))
            {
                curVehicle->setThrottle(1.0);
            }
            else if (getInput()->IsKeyPressed(Actions::button_Backward))
            {
                curVehicle->setThrottle(-1.0);
            }
            else
            {
                curVehicle->setThrottle(0.0);
            }

            float steering = getInput()->IsKeyPressed(Actions::button_TurnLeft) ? 0.3f : getInput()->IsKeyPressed(Actions::button_TurnRight) ? -0.3f : 0.0f;
            curVehicle->setSteeringValue(steering);

            player->setPosition(curVehicle->getPosition());
        }
        else
        {
            glm::vec3 playerPosition = player->getPosition();
            if (playerPosition.z <= -50)
            {
                glm::vec3 rayFrom(playerPosition.x, playerPosition.y, 300.f);
                glm::vec3 rayTo(playerPosition.x, playerPosition.y, 0.f);

                auto result = getWorld()->getPhysicsSystem()->rayCast(rayFrom, rayTo);

                if (result.HasHit)
                {
                    auto& ws = result.HitPos;
                    player->setPosition(glm::vec3(ws.x, ws.y, ws.z + 10.0f));
                }
            }
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
                btVector3(movement.x * 400.0f * speed * delta_time, movement.y * 400.0f * speed * delta_time, player->getPhysCharacter()->getLinearVelocity().z()));

            if (getInput()->IsKeyTriggered(Actions::button_SPACE))
            {
                //	player->Jump();

                glm::vec3 m_rayStart = player->getPosition();
                glm::vec3 m_rayEnd = m_rayStart - glm::vec3(0.0, 0.0, 1.5);

                auto result = getWorld()->getPhysicsSystem()->rayCast(m_rayStart, m_rayEnd);
                if (result.HasHit)
                {
                    player->getPhysCharacter()->applyCentralImpulse(btVector3(0.f, 0.f, 50.0f));
                }
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

void Game::changeLocation()
{
    uint32_t random = rand() % getWorld()->getGameData()->Scenes.size();
    CPed* ped = getWorld()->getCurrentPlayer();

    ped->setPosition(getWorld()->getGameData()->Scenes[random]);
    // ped->setGravity(getWorld()->getPhysicsSystem()->getGravity());
    /*for (int i = 0; i < 3; i++)
    {
        if (getWorld()->currentPlayerID != i)
        {
            getWorld()->peds[i]->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
        }
    }*/
}
