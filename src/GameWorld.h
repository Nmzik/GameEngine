#pragma once

#include "includes.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "PhysicsDebugDrawer.h"
#include <btBulletDynamicsCommon.h>
#include "GameData.h"
#include "SpaceGrid.h"
//////////
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"

class ResourceManager;
class YmapLoader;
class Water;
class Camera;
class CPed;
class CVehicle;
class YdrLoader;
class YddLoader;
class YtdLoader;
class YbnLoader;

class GameWorld
{
	struct RenderInstruction {
		YdrLoader* ydr;
		glm::mat4 modelMatrix;

		RenderInstruction(YdrLoader* modelYdr, glm::mat4 Matrix) :ydr(modelYdr), modelMatrix(Matrix) {

		}
	};

	typedef std::vector<RenderInstruction> RenderList;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	GameData data;
	SpaceGrid spaceGrid;
	NodeGrid nodeGrid;
	std::unique_ptr<ResourceManager> _ResourceManager;
	//SoundManager sound;
	PhysicsDebugDrawer debug;

public:
	static btDiscreteDynamicsWorld* dynamicsWorld;

	YddLoader* skydome;
	uint32_t culled = 0;
	float LODMultiplier = 1.0f;
	std::vector<Water> WaterMeshes;
	//std::vector<YndLoader> yndLoader;
	std::vector<YtypLoader*> ytypLoader;
	std::unordered_map<uint32_t, CarHandling> vehiclesPool;
	std::vector<CPed> peds;
	std::vector<CVehicle> vehicles;

	std::mutex resources_lock;
	std::vector<Resource> resources;

	RenderList renderList;

	float accumulatedTime = 0.0f;

	glm::i32vec2 CurCell;
	glm::i32vec2 CurNodeCell;
	std::vector<YmapLoader*> CurYmaps;
	std::vector<YbnLoader*> CurYbns;
	bool EnableStreaming = true;
	uint8_t gameMinute;
	uint8_t gameHour;
	uint8_t currentPlayerID = 0;

	GameWorld();
	~GameWorld();

	void LoadYmap(YmapLoader* map, Camera* camera, glm::vec3& position);

	void GetVisibleYmaps(Camera* camera);

	void LoadQueuedResources();

	GameData* getGameData() {
		return &data;
	}

	ResourceManager* GetResourceManager() {
		return _ResourceManager.get();
	}

	btDiscreteDynamicsWorld* GetDynamicsWorld() {
		return dynamicsWorld;
	}

	PhysicsDebugDrawer* getDebugDrawer() {
		return &debug;
	}

	void createPedestrian();
	void createVehicle(glm::vec3 position);
	void UpdateDynamicObjects();
	void Update();
	void UpdateTraffic(Camera* camera, glm::vec3 pos);
	CVehicle* FindNearestVehicle();
	void DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection);
	void update(float delta_time, Camera* camera);

	void TestFunction(glm::vec3 Position);
	bool DetectInWater(glm::vec3 Position);
	void ClearTestFunction();
};
