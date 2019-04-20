#pragma once

#include <deque>
#include "CPed.h"
#include "CVehicle.h"
#include "Light.h"
#include "PhysicsSystem.h"
#include "SoundManager.h"
#include "SpaceGrid.h"
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
class GameData;

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

    float accumulatedTime = 0.0f;

    glm::i32vec2 CurCell;
    glm::i32vec2 CurNodeCell;
    glm::i32vec2 CurNavCell;
    std::vector<YmapLoader*> CurYmaps;
    std::vector<YbnLoader*> CurYbns;

public:
    YddLoader* skydome;
    uint32_t culled = 0;
    float LODMultiplier = 1.0f;
    uint8_t currentPlayerID = 0;
    //std::vector<Water> WaterMeshes;
    //std::unordered_map<uint32_t, CarHandling> vehiclesPool;
    std::vector<CPed*> peds;
    std::vector<CVehicle*> vehicles;

    std::mutex resources_lock;
    std::deque<Resource*> resources;
    std::deque<Resource*> resourcesThread;

    //	RenderList renderList;
    std::vector<Object*> renderList;

    bool EnableStreaming = true;
    DirectionalLight dirLight;
    uint8_t gameMinute;
    uint8_t gameHour;

    GameWorld(GameData* _gameData);
    ~GameWorld();

    void updateObjects(Camera* camera, glm::vec3& position);
    void getVisibleYmaps(glm::vec3& PlayerPos);
    void loadQueuedResources();

    CPed* getCurrentPlayer()
    {
        return peds[currentPlayerID];
    }

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

    /*PhysicsDebugDrawer& getDebugDrawer()
    {
        return physicsSystem.getDebugDrawer();
    }*/

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

    void AddVehicleToWorld(glm::vec3 position, float mass, YftLoader* model);
    void RemoveVehicleFromWorld(CVehicle* vehicle);
    void AddPedToWorld(glm::vec3 pos, YddLoader* model);
    void RemovePedFromWorld(CPed* ped);
};
