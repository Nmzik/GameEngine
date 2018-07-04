#pragma once
#include <btBulletDynamicsCommon.h>
#include "YdrLoader.h"

class YbnLoader
{
	struct Bounds {
		uint8_t Type;
		uint8_t Unknown_11h;
		uint16_t Unknown_12h;
		float BoundingSphereRadius;
		uint32_t Unknown_18h;
		uint32_t Unknown_1Ch;
		glm::vec3 BoundingBoxMax;
		float Margin;
		glm::vec3 BoundingBoxMin;
		uint32_t Unknown_3Ch;
		glm::vec3 BoundingBoxCenter;
		uint8_t MaterialIndex;
		uint8_t ProceduralId;
		uint8_t RoomId_and_PedDensity; //5bits for RoomID and then 3bits for PedDensity
		uint8_t Unknown_4Fh; //flags? (bit5 related to Unknown_5Ch, should be a flag called "Has PolyFlags")<-- i don't remember why i wrote this lol
		glm::vec3 Center;
		uint8_t PolyFlags;
		uint8_t MaterialColorIndex;
		uint16_t Unknown_5Eh;
		float Unknown_60h;
		float Unknown_64h;
		float Unknown_68h;
		float BoundingBoxVolume;
	};

	struct BoundGeometry {
		uint32_t Unknown_70h;
		uint32_t Unknown_74h;
		uint64_t Unknown_78h_Pointer;
		uint32_t Unknown_80h;
		uint32_t Count1;
		uint64_t PolygonsPointer;
		glm::vec3 Quantum;
		float Unknown_9Ch;
		glm::vec3 CenterGeom;
		float Unknown_ACh;
		uint64_t VerticesPointer;
		uint64_t Unknown_B8h_Pointer;
		uint64_t Unknown_C0h_Pointer;
		uint64_t Unknown_C8h_Pointer;
		uint32_t VerticesCount;
		uint32_t PolygonsCount;
		uint32_t Unknown_D8h; // 0x00000000
		uint32_t Unknown_DCh; // 0x00000000
		uint32_t Unknown_E0h; // 0x00000000
		uint32_t Unknown_E4h; // 0x00000000
		uint32_t Unknown_E8h; // 0x00000000
		uint32_t Unknown_ECh; // 0x00000000
		uint64_t MaterialsPointer;
		uint64_t MaterialColoursPointer;
		uint32_t Unknown_100h; // 0x00000000
		uint32_t Unknown_104h; // 0x00000000
		uint32_t Unknown_108h; // 0x00000000
		uint32_t Unknown_10Ch; // 0x00000000
		uint32_t Unknown_110h; // 0x00000000
		uint32_t Unknown_114h; // 0x00000000
		uint64_t PolygonMaterialIndicesPointer;
		uint8_t MaterialsCount;
		uint8_t MaterialColoursCount;
		uint16_t Unknown_122h; // 0x0000
		uint32_t Unknown_124h; // 0x00000000
		uint32_t Unknown_128h; // 0x00000000
		uint32_t Unknown_12Ch; // 0x00000000
	};

	enum BoundPolygonType
	{
		Triangle = 0,
		Sphere = 1,
		Capsule = 2,
		Box = 3,
		Cylinder = 4,
	};

	struct BoundPolygonTriangle {
		float triArea;
		uint16_t triIndex1;
		uint16_t triIndex2;
		uint16_t triIndex3;
		int16_t edgeIndex1;
		int16_t edgeIndex2;
		int16_t edgeIndex3;
	};

	struct BoundPolygonSphere {
		uint16_t sphereType;
		uint16_t sphereIndex;
		float sphereRadius;
		uint32_t unused0;
		uint32_t unused1;
	};

	struct BoundPolygonCapsule {
		uint16_t capsuleType;
		uint16_t capsuleIndex1;
		float capsuleRadius;
		uint16_t capsuleIndex2;
		uint16_t unused0;
		uint32_t unused1;
	};

	struct BoundPolygonBox {
		uint32_t boxType;
		int16_t boxIndex1;
		int16_t boxIndex2;
		int16_t boxIndex3;
		int16_t boxIndex4;
		uint32_t unused0;
	};

	struct BoundPolygonCylinder {
		uint16_t cylinderType;
		uint16_t cylinderIndex1;
		float cylinderRadius;
		uint16_t cylinderIndex2;
		uint16_t unused0;
		uint32_t unused1;
	};

	struct BoundComposite {
		uint64_t ChildrenPointer;
		uint64_t ChildrenTransformation1Pointer;
		uint64_t ChildrenTransformation2Pointer;
		uint64_t ChildrenBoundingBoxesPointer;
		uint64_t Unknown_90h_Pointer;
		uint64_t Unknown_98h_Pointer;
		uint16_t ChildrenCount1;
		uint16_t ChildrenCount2;
		uint32_t Unknown_A4h; // 0x00000000
		uint64_t BVHPointer;
	};

	std::vector<glm::u16vec3*> IndicesArray;
	std::vector<glm::vec3*> VerticesArray;

	btDiscreteDynamicsWorld* CollisionWorld;
	std::vector<btCollisionShape*> Shapes;
	std::vector<btTriangleIndexVertexArray*> VertIndicesArray;
	std::vector<btBvhTriangleMeshShape*> trishapes;
public:
	YbnLoader * next;

	btCompoundShape* compound = nullptr;
	btRigidBody * rigidBody = nullptr;
	uint32_t RefCount = 0;
	bool Loaded = false;

	void Init(memstream2 & file);
	void ParseYbn(memstream2& file);
	void Finalize(btDiscreteDynamicsWorld* world);
	void Remove();
};

class YbnPool
{
public:
	YbnPool();
	~YbnPool();

	YbnLoader* Load();
	void Remove(YbnLoader* ybn);

	YbnLoader ybns[50];

private:
	YbnLoader * firstAvailable_;
};
