#include "Game.h"

#include "CPed.h"
#include "CVehicle.h"
#include "Camera.h"
#include "GTAEncryption.h"
#include "GameData.h"
#include "ResourceManager.h"
#include "WorldRenderer.h"

#include "loaders/YbnLoader.h"
#include "loaders/YdrLoader.h"
#include "loaders/YtdLoader.h"

#ifdef WIN32
#include "windows/Win32Window.h"
#endif

#ifdef __ANDROID__
#include "android/AndroidWindow.h"
#endif

#include "GameWorld.h"
#include "InputActions.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "ScriptInterpreter.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

extern FreeListAllocator* physicsAllocator;

Game::Game(const char* Path, const char* keysPath)
    : paused(false)
    , gameTime(0)
{
    gameData = std::make_unique<GameData>(Path, keysPath);
    gameData->load();
    resourceManager = std::make_unique<ResourceManager>(gameData.get(), this);
    gameWorld = std::make_unique<GameWorld>(resourceManager.get());
#ifdef WIN32
    window = std::make_unique<Win32Window>();
#endif
#ifdef __ANDROID__
    window = std::make_unique<AndroidWindow>();
#endif
    rendering_system = std::make_unique<WorldRenderer>(window.get());
    input = std::make_unique<InputManager>();
    scriptMachine = std::make_unique<ScriptInterpreter>(gameData.get(), this);

    //scriptMachine->startThread(GenHash("startup_install"));
#if (WIN32)
    window->addListener(input.get());

    initializeCamera(1280.f, 720.f);
#endif

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
    current_time = std::chrono::steady_clock::now();
}

Game::~Game()
{
}

void Game::postLoad()
{
    gameWorld->postLoad();
    rendering_system->postLoad();
}

void Game::initializeCamera(float width, float height)
{
//#ifdef METAL
#ifdef __APPLE__
    camera = std::make_unique<Camera>(glm::vec3(0.0, 0.0, 100.0), glm::perspective(glm::radians(45.0f), (float)width / (float)height, 10000.0f, 0.1f));
#else
    camera = std::make_unique<Camera>(glm::vec3(0.0, 0.0, 100.0), glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f));
#endif
}

void Game::run()
{
    bool running = true;

    while (running)
    {
        frame();
    }
}

void Game::frame()
{
    auto cpuThreadStart = std::chrono::steady_clock::now();

    auto old_time = current_time;
    current_time = std::chrono::steady_clock::now();
    float delta_time = std::chrono::duration<float>(current_time - old_time).count();
    gameTime += delta_time;

    input->update();
#if (WIN32)
    window->processEvents();
#endif

    /*if (input->isKeyPressed(Actions::button_ESCAPE))
        break;*/
    //running = false;

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

    resourceManager->loadQueuedResources();
    resourceManager->updateResourceCache(gameWorld.get());

    updateFPS(delta_time, cpuThreadTime, gpuThreadTime);
}

void Game::updateFPS(float delta_time, float cpuThreadTime, float gpuThreadTime)
{
    /*static auto time_since_last_fps_output = 0.f;

    time_since_last_fps_output += delta_time;
    if (time_since_last_fps_output >= 1.0f)
    {
        time_since_last_fps_output = 0.0f;

        osstr << "Game "
              << (1.0f / delta_time) << " FPS, "
              << (delta_time * 1000.0f) << " CPU time, "
              << (cpuThreadTime * 1000.0f) << " CPU Thread time, "
              << (gpuThreadTime * 1000.0f) << " Render Thread time, "
              //<< rendering_system->gpuTime * 0.000001f << " GPU time, "
              << gameWorld->culledYmaps << " Culled ymaps, "
              << gameWorld->renderList.size() << " Objects, "
              << rendering_system->getNumDrawCalls() << " Draw Calls, "
              //<< resourceManager->GlobalGpuMemory / 1024 / 1024 << " MB GPU Mem, "
              //<< resourceManager->TextureMemory / 1024 / 1024 << " MB Texture Mem, "
              << (physicsAllocator->getSize() - physicsAllocator->getUsedMemory()) / 1024 / 1024 << " MB Bullet Free Mem ";
        //window->setTitle(osstr.str());
    }*/
}

void Game::tick(float delta_time)
{
    camera->setLookDirection(getInput()->getMouseMovement());

    if (getInput()->isKeyTriggered(Actions::button_E))
    {
        printf("CUR POS PLAYER %s\n", glm::to_string(getWorld()->getCurrentPlayer()->getPosition()).c_str());
        //camera->setPosition(glm::vec3(0.f));
        //	getWorld()->LODMultiplier += 0.05f;
        //getWorld()->EnableStreaming = !getWorld()->EnableStreaming;
    }

    /*if (getInput()->isKeyTriggered(Actions::button_GPU_TIME))
     {
     getRenderer()->gpuTimer = !getRenderer()->gpuTimer;
     //clear variable
     getRenderer()->gpuTime = 0;
     }*/

    /*if (getInput()->isKeyTriggered(Actions::button_Z))
     {
     if (getWorld()->gameHour > 23)
     getWorld()->gameHour = 0;
     getWorld()->gameHour++;
     }
     
     if (getInput()->isKeyTriggered(Actions::button_X))
     {
     if (getWorld()->gameHour < 1)
     getWorld()->gameHour = 23;
     getWorld()->gameHour--;
     //	getRenderer()->ShowTexture = !getRenderer()->ShowTexture;
     }*/

    if (getInput()->isKeyPressed(Actions::button_player1))
    {
        getWorld()->currentPlayerID = 0;

        changeLocation();
    }

    if (getInput()->isKeyTriggered(Actions::button_player2))
		getWorld()->currentPlayerID = 1;

    if (getInput()->isKeyTriggered(Actions::button_player3))
		getWorld()->currentPlayerID = 2;

    if (getInput()->isKeyTriggered(Actions::button_ShowCollision))
        getRenderer()->renderDebugWorld = !getRenderer()->renderDebugWorld;

    CPed* player = getWorld()->getCurrentPlayer();

    if (getInput()->isKeyTriggered(Actions::button_CameraMode))
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

    if (getInput()->isKeyTriggered(Actions::button_EnterExitVehicle))
    {
        if (player->getCurrentVehicle())
        {
            player->setPosition(player->getCurrentVehicle()->getPosition());

            player->exitVehicle();
            getWorld()->getPhysicsSystem()->wakeRigidBodyFromSleep(player->getPhysCharacter());
        }
        else
        {
            player->enterVehicle(getWorld()->findNearestVehicle());
            if (player->getCurrentVehicle())
            {
                getWorld()->getPhysicsSystem()->putRigidBodyToSleep(player->getPhysCharacter());
            }
        }
    }

    float speed = getInput()->isKeyPressed(Actions::button_LSHIFT) ? 80.0f : 10.0f;

    glm::vec3 movement{};
    movement.x = (float)getInput()->isKeyPressed(Actions::button_Forward) - getInput()->isKeyPressed(Actions::button_Backward);
    movement.y = (float)getInput()->isKeyPressed(Actions::button_TurnLeft) - getInput()->isKeyPressed(Actions::button_TurnRight);

    movement *= speed;

    if (camera->getCameraMode() == CameraMode::FreeCamera)
    {
        glm::vec3 camPos = camera->getPosition();
        if (getInput()->isKeyPressed(Actions::button_CameraUp))
        {
            camPos.z += 2;
        }

        if (getInput()->isKeyPressed(Actions::button_CameraDown))
        {
            camPos.z -= 2;
        }

        camera->setPosition(camPos + movement);
    }
    else
    {
        if (CVehicle * curVehicle; (curVehicle = player->getCurrentVehicle()))
        {
            glm::vec3 vehiclePosition = curVehicle->getPosition();

            if (vehiclePosition.z <= -150)
            {
                glm::vec3 rayFrom(vehiclePosition.x, vehiclePosition.y, 300.f);
                glm::vec3 rayTo(vehiclePosition.x, vehiclePosition.y, 0.f);

                auto result = getWorld()->getPhysicsSystem()->rayCast(rayFrom, rayTo);

                if (result.HasHit)
                {
                    auto& ws = result.HitPos;
                    curVehicle->setPosition(glm::vec3(ws.x, ws.y, ws.z + 10.f));
                    curVehicle->getCarChassisRigidbody()->clearForces();
                }
            }

            curVehicle->setThrottle(movement.x);
            curVehicle->setSteeringValue(movement.y);
            player->setPosition(vehiclePosition);
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
                    player->setPosition(glm::vec3(ws.x, ws.y, ws.z + 5.0f));
                    player->setNeedsReset();
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

            player->setMovementDirection(movement);

            if (getInput()->isKeyTriggered(Actions::button_SPACE))
            {
                //	player->Jump();

                glm::vec3 m_rayStart = player->getPosition();
                glm::vec3 m_rayEnd = m_rayStart - glm::vec3(0.0, 0.0, 1.5);

                auto result = getWorld()->getPhysicsSystem()->rayCast(m_rayStart, m_rayEnd);
                if (result.HasHit)
                {
                    player->getPhysCharacter()->applyCentralImpulse(btVector3(0.f, 0.f, 5.0f));
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
    uint32_t random = rand() % resourceManager->getGameData()->scenes.size();
    CPed* ped = getWorld()->getCurrentPlayer();

    ped->setPosition(resourceManager->getGameData()->scenes[0]);
    // ped->setGravity(getWorld()->getPhysicsSystem()->getGravity());
    /*for (int i = 0; i < 3; i++)
     {
     if (getWorld()->currentPlayerID != i)
     {
     getWorld()->peds[i]->getPhysCharacter()->setGravity(btVector3(0, 0, 0));
     }
     }*/
}
