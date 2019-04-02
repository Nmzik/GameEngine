#pragma once

#include "CPed.h"
#include "CVehicle.h"
#include "GameData.h"
#include "Light.h"
#include "PhysicsSystem.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "SpaceGrid.h"
#include "includes.h"
//////////
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"

class ResourceManager;
class YmapLoader;
class Water;
class Camera;
class YdrLoader;
class YbnLoader;
class Object;

inline float RandomFloat(float min, float max)
{
    return (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

class GameWorld
{
    struct RenderInstruction
    {
        YdrLoader* ydr;
        glm::mat4 modelMatrix;

        RenderInstruction(YdrLoader* modelYdr, glm::mat4 Matrix)
            : ydr(modelYdr)
            , modelMatrix(Matrix)
        {
        }
    };

    using RenderList = std::vector<RenderInstruction>;

    GameData& data;
    SpaceGrid spaceGrid;
    NodeGrid nodeGrid;
    NavGrid navGrid;

    PhysicsSystem physicsSystem;
    std::unique_ptr<ResourceManager> resourceManager;
    //	SoundManager sound;

public:
    YddLoader* skydome;
    uint32_t culled = 0;
    float LODMultiplier = 1.0f;
    //std::vector<Water> WaterMeshes;
    //std::unordered_map<uint32_t, CarHandling> vehiclesPool;
    std::vector<CPed*> peds;
    std::vector<CVehicle*> vehicles;

    std::mutex resources_lock;
    std::deque<Resource*> resources;
    std::deque<Resource*> resourcesThread;

    //	RenderList renderList;
    std::vector<Object*> renderList;

    float accumulatedTime = 0.0f;

    glm::i32vec2 CurCell;
    glm::i32vec2 CurNodeCell;
    glm::i32vec2 CurNavCell;
    std::vector<YmapLoader*> CurYmaps;
    std::vector<YbnLoader*> CurYbns;
    bool EnableStreaming = true;

    DirectionalLight dirLight;
    uint8_t gameMinute;
    uint8_t gameHour;
    uint8_t currentPlayerID = 0;

    GameWorld(GameData* _gameData);
    ~GameWorld();

    void loadYmap(YmapLoader* map, Camera* camera, glm::vec3& position);
    void getVisibleYmaps(Camera* camera);
    void loadQueuedResources();

    PhysicsSystem* getPhysicsSystem()
    {
        return &physicsSystem;
    }

    GameData* getGameData() const
    {
        return &data;
    }

    ResourceManager* getResourceManager() const
    {
        return resourceManager.get();
    }

    PhysicsDebugDrawer& getDebugDrawer()
    {
        return physicsSystem.getDebugDrawer();
    }

    void createPedestrian();
    void createVehicle(glm::vec3 position);
    void updateDynamicObjects();
    void cleanupTraffic(Camera* camera);
    void createTraffic(Camera* camera);

    CVehicle* findNearestVehicle();
    void detectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection);
    void updateWorld(float delta_time, Camera* camera);

    void testFunction(glm::vec3 Position);
    bool detectInWater(glm::vec3 Position);
};
