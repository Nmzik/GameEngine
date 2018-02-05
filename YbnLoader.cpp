#include "YbnLoader.h"

YbnLoader::YbnLoader(btDiscreteDynamicsWorld* world)
{
	std::ifstream file("C:\\Users\\nmzik\\Desktop\\sm_smugdlc_int_01.ybn", std::ios::binary);

	if (!file.is_open()) {
		printf("NOT FOUND!");
	}

	btCompoundShape* compound = new btCompoundShape();
	std::vector<glm::vec3> FinalVertices;
	std::vector<glm::vec3> FinalNormals;

	struct {
		uint32_t FileVFT;
		uint32_t FileUnknown;
		uint64_t FilePagesInfoPointer;
	} ResourceFileBase;
	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

	uint64_t SYSTEM_BASE = 0x50000000;
	uint64_t GRAPHICS_BASE = 0x60000000;

	struct {
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



	} Bounds;

	file.read((char*)&Bounds, sizeof(Bounds));

	//file.seekg(16, std::ios::cur);

	if (Bounds.Type != 10) printf("NEED FIX YBN!");
	/*uint8_t type;
	file.read((char*)&type, sizeof(uint8_t));
	printf("TYPE %d", type);*/

	struct {
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
	} BoundComposite;

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
		std::vector<glm::vec3> Vertices;
	};

	struct Vertex_HalfType{
		int16_t x;
		int16_t y;
		int16_t z;
	};

	std::vector<BoundGeometry> BoundGeometries;

	file.read((char*)&BoundComposite, sizeof(BoundComposite));

	if ((BoundComposite.ChildrenPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		BoundComposite.ChildrenPointer = BoundComposite.ChildrenPointer & ~0x50000000;
	}
	if ((BoundComposite.ChildrenPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		BoundComposite.ChildrenPointer = BoundComposite.ChildrenPointer & ~0x60000000;
	}

	file.seekg(BoundComposite.ChildrenPointer);

	for (int i = 0; i < BoundComposite.ChildrenCount1; i++)
	{
		uint64_t DataPointer;
		file.read((char*)&DataPointer, sizeof(uint64_t));

		uint64_t BoundsPointer = file.tellg();

		if ((DataPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			DataPointer = DataPointer & ~0x50000000;
		}
		if ((DataPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			DataPointer = DataPointer & ~0x60000000;
		}

		file.seekg(DataPointer);

		struct {
			uint32_t FileVFT;
			uint32_t FileUnknown;
			uint64_t FilePagesInfoPointer;
		} ResourceFileBase;
		file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

		file.read((char*)&Bounds, sizeof(Bounds));
		printf("TYPE %d\n", Bounds.Type);

		BoundGeometry geom;
		file.read((char*)&geom, sizeof(BoundGeometry) -24);

		/////////////////READ POLYGONS
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
			//////
			int32_t vertIndex1;
			int32_t vertIndex2;
			int32_t vertIndex3;
		};

		std::vector<BoundPolygonTriangle> PolygonTriangles;
		CenterGeometry.push_back(geom.CenterGeom);
		if ((geom.PolygonsPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			geom.PolygonsPointer = geom.PolygonsPointer & ~0x50000000;
		}
		if ((geom.PolygonsPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			geom.PolygonsPointer = geom.PolygonsPointer & ~0x60000000;
		}

		file.seekg(geom.PolygonsPointer);

		//uint8_t* PolygonsData = new uint8_t[geom.PolygonsCount * 16];
		//file.read((char*)&PolygonsData[0], geom.PolygonsCount * 16);

		for (int i = 0; i < geom.PolygonsCount; i++)  //PERFORMANCE IMPROVEMENT???
		{
			uint8_t type;
			file.read((char*)&type, sizeof(uint8_t));
			file.seekg(-1, std::ios::cur);

			switch (type & 7)
			{
			case 0:
				BoundPolygonTriangle triangle;
				file.read((char*)&triangle, sizeof(BoundPolygonTriangle) - 12);
				triangle.vertIndex1 = triangle.triIndex1 & 0x7FFF;
				triangle.vertIndex2 = triangle.triIndex2 & 0x7FFF;
				triangle.vertIndex3 = triangle.triIndex3 & 0x7FFF;
				PolygonTriangles.push_back(triangle);
				break;
			default:
				file.seekg(16, std::ios::cur);
				//printf("ERROR NOT IMPLEMENTED!");
				break;
			}

			/*BoundPolygonType type = (BoundPolygonType)(PolygonsData[i * 16] & 7);

			switch (type)
			{
			case Triangle:
				BoundPolygonTriangle triangle;
				file.read((char*)&triangle, sizeof(BoundPolygonTriangle));
				PolygonTriangles.push_back(triangle);
				break;
			default:
				printf("UNDEFINED NEEDS FIX!");
				break;
			}*/
		}

		///////////////////

		if ((geom.VerticesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			geom.VerticesPointer = geom.VerticesPointer & ~0x50000000;
		}
		if ((geom.VerticesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			geom.VerticesPointer = geom.VerticesPointer & ~0x60000000;
		}

		file.seekg(geom.VerticesPointer);

		Vertex_HalfType *vertices = new Vertex_HalfType[geom.VerticesCount];
		file.read((char*)&vertices[0], sizeof(Vertex_HalfType) * geom.VerticesCount);

		geom.Vertices.resize(geom.VerticesCount);

		for (int i = 0; i < geom.VerticesCount; i++)
		{
			geom.Vertices[i] = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z) * geom.Quantum;
		}

		delete [] vertices;

		//FinalVertices.resize(PolygonTriangles.size());
		btTriangleMesh* TriMesh = new btTriangleMesh();
		for (int i = 0; i < PolygonTriangles.size(); i++)
		{
			glm::vec3 Vertex1 = geom.Vertices[PolygonTriangles[i].vertIndex1];
			FinalVertices.push_back(Vertex1);
			glm::vec3 Vertex2 = geom.Vertices[PolygonTriangles[i].vertIndex2];
			FinalVertices.push_back(Vertex2);
			glm::vec3 Vertex3 = geom.Vertices[PolygonTriangles[i].vertIndex3];
			FinalVertices.push_back(Vertex3);
			TriMesh->addTriangle(btVector3(Vertex1.x, Vertex1.y, Vertex1.z), btVector3(Vertex2.x, Vertex2.y, Vertex2.z), btVector3(Vertex3.x, Vertex3.y, Vertex3.z));
			//shape->addPoint(btVector3(Vertex1.x, Vertex1.y, Vertex1.z));
			//shape->addPoint(btVector3(Vertex2.x, Vertex2.y, Vertex2.z));
			//shape->addPoint(btVector3(Vertex3.x, Vertex3.y, Vertex3.z));
			glm::vec3 normal = glm::normalize(glm::cross(Vertex2 - Vertex1, Vertex3 - Vertex1));
			FinalNormals.push_back(normal);
			FinalNormals.push_back(normal);
			FinalNormals.push_back(normal);
		}
		if (TriMesh->getNumTriangles() != 0) {
			btBvhTriangleMeshShape* trishape =
				new btBvhTriangleMeshShape(TriMesh, false);


			if (FinalVertices.size() != 0) {
				btTransform localTrans;
				localTrans.setIdentity();
				//localTrans effectively shifts the center of mass with respect to the chassis
				localTrans.setOrigin(btVector3(geom.CenterGeom.x, geom.CenterGeom.y, geom.CenterGeom.z));
				compound->addChildShape(localTrans, trishape);
			}
		}

		Mesh mesh(FinalVertices, FinalNormals);
		meshes.push_back(mesh);

		FinalVertices.clear();
		FinalVertices.shrink_to_fit();

		FinalNormals.clear();
		FinalNormals.shrink_to_fit();

		BoundGeometries.push_back(geom);

		file.seekg(BoundsPointer);
	}

	btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(0.707f, 0.f, 0.f, -0.707f), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, MotionState, compound, btVector3(0, 0, 0));
	btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
	world->addRigidBody(rigidBody);
}


YbnLoader::~YbnLoader()
{
}

glm::mat4 YbnLoader::GetMat4()
{
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(25.8109264, -8.294803, 9.779061));
	return model;
}

void YbnLoader::Draw()
{
	for (int i = 0; i < 1; i++) {
		meshes[i].DrawCollision();
	}
}