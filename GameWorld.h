#pragma once
#include "SoundManager.h"
#include "Model.h"
#include "Player.h"
#include "Vehicle.h"
#include "ResourceManager.h"
#include "PhysicsDebugDrawer.h"
#include "YdrLoader.h"
#include "YddLoader.h"
#include "YmapLoader.h"
#include "YndLoader.h"
#include "YbnLoader.h"
#include "GameData.h"
#include <windows.h>

class ResourceManager;

class GameWorld
{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	GameData data;
	ResourceManager* _ResourceManager;
	SoundManager sound;
	PhysicsDebugDrawer debug;

public:
	std::vector<YdrLoader> ydrLoader;
	std::vector<YddLoader> yddLoader;
	std::vector<YndLoader> yndLoader;
	std::vector<YbnLoader> ybnLoader;
	std::vector<Model*> models;
	std::vector<Player*> pedestrians;
	std::vector<Vehicle> vehicles;
	uint8_t gameMinute;
	uint8_t gameHour;
	Player* player;

	GameWorld();
	~GameWorld();

	void LoadYmap(uint32_t hash);

	bool LoadYDD(uint32_t hash);

	bool LoadYDR(uint32_t hash, glm::vec3 position);

	ResourceManager* GetResourceManager() {
		return _ResourceManager;
	}

	btDiscreteDynamicsWorld* GetDynamicsWorld() {
		return dynamicsWorld;
	}

	PhysicsDebugDrawer* getDebugDrawer() {
		return &debug;
	}

	bool LoadYDR(uint32_t hash);
	void createPedestrian();
	void createVehicle();
	void Update();
	void UpdateTraffic(glm::vec3 cameraPosition);
	Vehicle* FindNearestVehicle();
	void DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection);
	void update();
};

