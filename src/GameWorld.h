#pragma once

#include "includes.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "GameData.h"
#include "SpaceGrid.h"
#include "Light.h"
#include "PhysicsSystem.h"
//////////
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"

#include "CPed.h"

class ResourceManager;
class YmapLoader;
class Water;
class Camera;
class CVehicle;
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

	typedef std::vector<RenderInstruction> RenderList;

	GameData data;
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
	std::vector<Water> WaterMeshes;
	std::unordered_map<uint32_t, CarHandling> vehiclesPool;
	std::vector<CPed> peds;
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

	GameWorld();
	~GameWorld();

	void LoadYmap(YmapLoader* map, Camera* camera, glm::vec3& position);

	void GetVisibleYmaps(Camera* camera);

	void LoadQueuedResources();

	GameData* getGameData()
	{
		return &data;
	}

	ResourceManager* GetResourceManager()
	{
		return resourceManager.get();
	}

	PhysicsDebugDrawer& getDebugDrawer()
	{
		return physicsSystem.getDebugDrawer();
	}

	void createPedestrian();
	void createVehicle(glm::vec3 position);
	void UpdateDynamicObjects();
	void UpdateTraffic(Camera* camera, glm::vec3 pos);
	CVehicle* FindNearestVehicle();
	void DetectWeaponHit(glm::vec3 CameraPosition, glm::vec3 lookDirection);
	void update(float delta_time, Camera* camera);

	void TestFunction(glm::vec3 Position);
	bool DetectInWater(glm::vec3 Position);
	void ClearTestFunction();
};
