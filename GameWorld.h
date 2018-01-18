#pragma once
#include "SoundManager.h"
#include "Model.h"
#include "Player.h"
#include "Vehicle.h"
#include "ResourceManager.h"
#include "PhysicsDebugDrawer.h"

class ResourceManager;

class GameWorld
{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	ResourceManager* _ResourceManager;
	SoundManager sound;
	PhysicsDebugDrawer debug;

public:
	std::vector<Model> models;
	std::vector<Player> pedestrians;
	std::vector<Vehicle> vehicles;
	uint8_t gameMinute;
	uint8_t gameHour;
	Player* player;

	GameWorld();
	~GameWorld();

	btDiscreteDynamicsWorld* GetDynamicsWorld() {
		return dynamicsWorld;
	}

	PhysicsDebugDrawer* getDebugDrawer() {
		return &debug;
	}

	void createPedestrian();
	void createVehicle();
	void UpdateTraffic(glm::vec3 cameraPosition);
	Vehicle* FindNearestVehicle();
	void DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection);
	void update();
};

