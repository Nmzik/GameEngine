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
#include <windows.h>
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
	std::vector<uint32_t> Hashes;

public:
	std::vector<Water> WaterMeshes;
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

	void LoadYmap(uint32_t hash, glm::vec3 cameraPosition);

	bool LoadYTYP(uint32_t hash);

	bool LoadYTD(uint32_t hash);

	bool LoadYDD(uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale);

	bool LoadYDR(uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale);

	bool LoadYFT(uint32_t hash, glm::vec3 position, glm::quat rotation, glm::vec3 scale);

	bool LoadYBN(uint32_t hash);

	void GetVisibleYmaps(glm::vec3 Position);

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

