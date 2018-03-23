#pragma once
#include "SoundManager.h"
#include "Model.h"
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
	SoundManager sound;
	PhysicsDebugDrawer debug;

public:
	YddLoader* skydome;
	uint32_t culled = 0;
	float LODMultiplier = 1.0f;
	std::vector<Water> WaterMeshes;
	std::unordered_map<uint32_t, YdrLoader*> ydrLoaderTest;
	std::unordered_map<uint32_t, YddLoader*> yddLoaderTest;
	std::unordered_map<uint32_t, YftLoader*> yftLoaderTest;
	std::unordered_map<uint32_t, YtdFile*> ytdLoaderTest;
	std::unordered_map<uint32_t, YbnLoader*> ybnLoaderTest;
	std::unordered_map<uint32_t, YmapLoader*> ymapLoaderTest;
	std::vector<YdrLoader*> ydrLoader;
	std::vector<YddLoader*> yddLoader;
	std::vector<YftLoader*> yftLoader;
	std::vector<YtdFile*> ytdLoader;
	std::vector<YndLoader> yndLoader;
	std::vector<YbnLoader*> ybnLoader;
	std::vector<YtypLoader*> ytypLoader;
	std::vector<YmapLoader*> ymapLoader;
	std::vector<Model*> models;
	std::vector<Player*> pedestrians;
	std::vector<Vehicle*> vehicles;
	uint8_t gameMinute;
	uint8_t gameHour;
	Player* player;

	GameWorld();
	~GameWorld();

	void LoadYmap(Shader* shader, uint32_t hash, Camera* camera);

	bool LoadYTYP(uint32_t hash);

	bool LoadYTD(uint32_t hash);

	void LoadYDR(Shader * shader, uint32_t hash, glm::mat4 & matrix);

	void LoadYDD(Shader * shader, uint32_t hash, uint32_t DrawableDictionaryHash, glm::mat4 & matrix);

	void LoadYFT(Shader * shader, uint32_t hash, glm::mat4 & matrix);

	bool LoadYBN(uint32_t hash);

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
	void UpdateTraffic(glm::vec3 cameraPosition);
	Vehicle* FindNearestVehicle();
	void DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection);
	void update(float delta_time);

	void TestFunction(glm::vec3 Position);
	bool DetectInWater(glm::vec3 Position);
	void ClearTestFunction();
};

