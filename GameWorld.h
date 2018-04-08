#pragma once
#include "SoundManager.h"
#include "Player.h"
#include "Vehicle.h"
#include "ResourceManager.h"
#include "PhysicsDebugDrawer.h"
#include "YdrLoader.h"
#include "YddLoader.h"
#include "YftLoader.h"
#include "YmapLoader.h"
#include "YtypLoader.h"
#include "YndLoader.h"
#include "YbnLoader.h"
#include "YtdLoader.h"
#include "GameData.h"
#include "Material.h"
#include "CacheDatFile.h"
#include "SpaceGrid.h"
#include "Water.h"
#include "Shader.h"
#include <windows.h>
#include "Camera.h"
#include "SDL.h"

class ResourceManager;

class GameWorld
{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	GameData data;
	CacheDatFile cacheFile;
	SpaceGrid spaceGrid;
	ResourceManager* _ResourceManager;
	//SoundManager sound;
	PhysicsDebugDrawer debug;

public:
	YddLoader* skydome;
	uint32_t culled = 0;
	float LODMultiplier = 1.0f;
	std::vector<Water> WaterMeshes;
	std::unordered_map<uint32_t, YdrLoader*> ydrLoader;
	std::unordered_map<uint32_t, YddLoader*> yddLoader;
	std::unordered_map<uint32_t, YftLoader*> yftLoader;
	std::unordered_map<uint32_t, YtdFile*> ytdLoader;
	//std::vector<YndLoader> yndLoader;
	std::unordered_map<uint32_t, YbnLoader*> ybnLoader;
	std::vector<YtypLoader*> ytypLoader;
	std::unordered_map<uint32_t, YmapLoader*> ymapLoader;
	std::vector<Player*> pedestrians;
	std::vector<Vehicle*> vehicles;
	bool testSpawn = true;
	uint8_t gameMinute;
	uint8_t gameHour;
	uint8_t currentPlayerID = 0;
	Player* player[3];
	YftLoader* vehicleModel;

	GameWorld();
	~GameWorld();

	void LoadYmap(Shader* shader, uint32_t hash, Camera* camera);

	bool LoadYTYP(uint32_t hash);

	void LoadYTD(uint32_t hash);

	void LoadYDR(Shader * shader, uint32_t hash, glm::mat4 & matrix);

	void LoadYDD(Shader * shader, uint32_t hash, uint32_t DrawableDictionaryHash, glm::mat4 & matrix);

	void LoadYFT(Shader * shader, uint32_t hash, glm::mat4 & matrix);

	void LoadYBN(uint32_t hash);

	void GetVisibleYmaps(Shader* shader, Camera* camera);

	ResourceManager* GetResourceManager() {
		return _ResourceManager;
	}

	btDiscreteDynamicsWorld* GetDynamicsWorld() {
		return dynamicsWorld;
	}

	PhysicsDebugDrawer* getDebugDrawer() {
		return &debug;
	}

	void createPedestrian();
	void createVehicle();
	void Update();
	void UpdateTraffic(Camera* camera);
	Vehicle* FindNearestVehicle();
	void DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection);
	void update(float delta_time);

	void TestFunction(glm::vec3 Position);
	bool DetectInWater(glm::vec3 Position);
	void ClearTestFunction();
};

