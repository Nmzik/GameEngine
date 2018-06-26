#include "YbnLoader.h"

void YbnLoader::Init(memstream2& file, btDiscreteDynamicsWorld* world)
{
	CollisionWorld = world;

	ResourceFileBase* resourceFileBase = (ResourceFileBase*)file.read(sizeof(ResourceFileBase));

	Bounds* bounds = (Bounds*)file.read(sizeof(Bounds));

	//Bounds->type = 3???? WTF IS THAT

	if (bounds->Type == 8 || bounds->Type == 4) {
		btCompoundShape* compound = new btCompoundShape();
		Shapes.push_back(compound);

		BoundGeometry* geom = (BoundGeometry*)file.read(sizeof(BoundGeometry));

		indices.reserve(geom->PolygonsCount);

		std::vector<BoundPolygonSphere*> PolygonSpheres;
		std::vector<BoundPolygonCapsule*> PolygonCapsules;
		std::vector<BoundPolygonBox*> PolygonBoxes;
		std::vector<BoundPolygonCylinder*> PolygonCylinders;

		SYSTEM_BASE_PTR(geom->PolygonsPointer);

		file.seekg(geom->PolygonsPointer);

		for (uint32_t i = 0; i < geom->PolygonsCount; i++)  //PERFORMANCE IMPROVEMENT???
		{
			uint8_t* type = (uint8_t*)file.read(sizeof(uint8_t));
			file.seekCur(-1);

			switch (type[0] & 7)
			{
			case 0: {
				BoundPolygonTriangle* PolygonTriangle = (BoundPolygonTriangle*)file.read(sizeof(BoundPolygonTriangle));
				indices.emplace_back(PolygonTriangle->triIndex1 & 0x7FFF, PolygonTriangle->triIndex2 & 0x7FFF, PolygonTriangle->triIndex3 & 0x7FFF);
				break;
			}
			case 1: {
				BoundPolygonSphere* PolygonSphere = (BoundPolygonSphere*)file.read(sizeof(BoundPolygonSphere));
				PolygonSpheres.push_back(PolygonSphere);
				break;
			}
			case 2: {
				BoundPolygonCapsule* PolygonCapsule = (BoundPolygonCapsule*)file.read(sizeof(BoundPolygonCapsule));
				PolygonCapsules.push_back(PolygonCapsule);
				break;
			}
			case 3: {
				BoundPolygonBox* PolygonBox = (BoundPolygonBox*)file.read(sizeof(BoundPolygonBox));
				PolygonBoxes.push_back(PolygonBox);
				break;
			}
			case 4: {
				BoundPolygonCylinder* PolygonCylinder = (BoundPolygonCylinder*)file.read(sizeof(BoundPolygonCylinder));
				PolygonCylinders.push_back(PolygonCylinder);
				break;
			}
			default:
				file.seekCur(16);
				//printf("ERROR NOT IMPLEMENTED!");
				break;
			}
		}


		///////////////////
		SYSTEM_BASE_PTR(geom->VerticesPointer);
		file.seekg(geom->VerticesPointer);

		glm::i16vec3 *vertices = (glm::i16vec3*)file.read(sizeof(glm::i16vec3));

		Vertices.resize(geom->VerticesCount);
		for (uint32_t i = 0; i < geom->VerticesCount; i++)
		{
			Vertices[i] = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z) * geom->Quantum;
		}

		///////////////////

		if (PolygonCylinders.size() != 0) {
			for (int i = 0; i < PolygonCylinders.size(); i++)
			{
				btCylinderShapeZ* shape = new btCylinderShapeZ(btVector3(0.5, 0.5, 0.5));
				Shapes.push_back(shape);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom->CenterGeom.x + Vertices[PolygonCylinders[i]->cylinderIndex1].x, geom->CenterGeom.y + Vertices[PolygonCylinders[i]->cylinderIndex1].y, geom->CenterGeom.z + Vertices[PolygonCylinders[i]->cylinderIndex1].z));
				compound->addChildShape(localTrans, shape);
			}
		}

		if (PolygonBoxes.size() != 0) {
			for (int i = 0; i < PolygonBoxes.size(); i++)
			{

				glm::vec3 p1 = Vertices[PolygonBoxes[i]->boxIndex1];
				glm::vec3 p2 = Vertices[PolygonBoxes[i]->boxIndex2];
				glm::vec3 p3 = Vertices[PolygonBoxes[i]->boxIndex3];
				glm::vec3 p4 = Vertices[PolygonBoxes[i]->boxIndex4];

				glm::vec3 test = glm::max(p1, p2);
				glm::vec3 test1 = glm::max(p3, p4);

				glm::vec3 test2 = glm::min(p1, p2);
				glm::vec3 test3 = glm::min(p3, p4);

				glm::vec3 max = glm::max(test, test1);
				glm::vec3 min = glm::min(test2, test3);

				auto size = (max - min) / 2.f;
				auto mid = (min + max) / 2.f;

				btBoxShape* shape = new btBoxShape(btVector3(-size.x, -size.y, size.z));

				Shapes.push_back(shape);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom->CenterGeom.x + mid.x, geom->CenterGeom.y + mid.y, geom->CenterGeom.z + mid.z));
				compound->addChildShape(localTrans, shape);
			}
		}

		if (PolygonCapsules.size() != 0) {
			for (int i = 0; i < PolygonCapsules.size(); i++) //Height is incorrect
			{
				auto mid = (Vertices[PolygonCapsules[i]->capsuleIndex1] + Vertices[PolygonCapsules[i]->capsuleIndex2]) / 2.f;

				btCapsuleShapeZ* capsule = new btCapsuleShapeZ(PolygonCapsules[i]->capsuleRadius, 1.0f);
				Shapes.push_back(capsule);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom->CenterGeom.x + mid.x, geom->CenterGeom.y + mid.y, geom->CenterGeom.z + mid.z));
				compound->addChildShape(localTrans, capsule);
			}
		}

		if (PolygonSpheres.size() != 0) {
			for (int i = 0; i < PolygonSpheres.size(); i++) //works as expected
			{
				btSphereShape* sphere = new btSphereShape(PolygonSpheres[i]->sphereRadius);
				Shapes.push_back(sphere);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom->CenterGeom.x + Vertices[PolygonSpheres[i]->sphereIndex].x, geom->CenterGeom.y + Vertices[PolygonSpheres[i]->sphereIndex].y, geom->CenterGeom.z + Vertices[PolygonSpheres[i]->sphereIndex].z));
				compound->addChildShape(localTrans, sphere);
			}
		}

		if (indices.size() != 0) {
			btIndexedMesh mesh;
			mesh.m_numTriangles = indices.size();
			mesh.m_triangleIndexBase = (uint8_t*)indices.data();
			mesh.m_triangleIndexStride = 3 * sizeof(uint16_t);
			mesh.m_numVertices = Vertices.size();
			mesh.m_vertexBase = (uint8_t*)Vertices.data();
			mesh.m_vertexStride = sizeof(glm::vec3);

			VertIndicesArray = new btTriangleIndexVertexArray();
			VertIndicesArray->addIndexedMesh(mesh, PHY_SHORT);

			/*btQuantizedBvh* quantizedBvh = new btQuantizedBvh();
			quantizedBvh->setQuantizationValues(btVector3(Bounds.BoundingBoxMin.x, Bounds.BoundingBoxMin.y, Bounds.BoundingBoxMin.z), btVector3(Bounds.BoundingBoxMax.x, Bounds.BoundingBoxMax.y, Bounds.BoundingBoxMax.z));
			QuantizedNodeArray&	nodes = quantizedBvh->getLeafNodeArray();
			btOptimizedBvh *bvh = new btOptimizedBvh();
			//bvh->build(VertIndicesArray, true, );
			trishape->setOptimizedBvh(bvh);
			btQuantizedBvhNode node;*/

			trishape = new btBvhTriangleMeshShape(VertIndicesArray, false);
			trishape->setMargin(bounds->Margin);

			btTransform localTrans;
			localTrans.setIdentity();
			localTrans.setOrigin(btVector3(geom->CenterGeom.x, geom->CenterGeom.y, geom->CenterGeom.z));
			compound->addChildShape(localTrans, trishape);
		}

		btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0, 0, 0)));
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, MotionState, compound, btVector3(0, 0, 0));
		rigidBody = new btRigidBody(groundRigidBodyCI);
		world->addRigidBody(rigidBody);
	}

	else if (bounds->Type == 10) {
		BoundComposite* boundComposite = (BoundComposite*)file.read(sizeof(BoundComposite));

		SYSTEM_BASE_PTR(boundComposite->ChildrenPointer);

		file.seekg(boundComposite->ChildrenPointer);

		ybns.reserve(boundComposite->ChildrenCount1);

		for (int i = 0; i < boundComposite->ChildrenCount1; i++)
		{
			uint64_t* DataPointer = (uint64_t*)file.read(sizeof(uint64_t));

			uint64_t BoundsPointer = file.tellg();

			SYSTEM_BASE_PTR(DataPointer[0]);

			file.seekg(DataPointer[0]);

			YbnLoader* ybn = new YbnLoader();
			ybn->Init(file, world);
			ybns.push_back(ybn);

			file.seekg(BoundsPointer);
		}
	}
}

void YbnLoader::Remove()
{
	for (auto& ybn : ybns)
	{
		ybn->Remove();
		delete ybn;
	}
	ybns.clear();

	for (auto& shape : Shapes)
	{
		delete shape;
	}

	Shapes.clear();

	if (trishape) {
		delete VertIndicesArray;
		VertIndicesArray = nullptr;
		delete trishape;
		trishape = nullptr;
	}

	Vertices.clear();
	indices.clear();

	if (rigidBody) {
		delete rigidBody->getMotionState();

		CollisionWorld->removeRigidBody(rigidBody);

		delete rigidBody;

		rigidBody = nullptr;
	}
}

YbnPool::YbnPool()
{
	firstAvailable_ = &ybns[0];

	for (int i = 0; i < 49; i++)
	{
		ybns[i].next = &ybns[i + 1];
	}

	ybns[49].next = NULL;
}

YbnPool::~YbnPool()
{
}

YbnLoader * YbnPool::Load()
{
	// Make sure the pool isn't full.
	assert(firstAvailable_ != NULL);

	// Remove it from the available list.
	YbnLoader * newYbn = firstAvailable_;
	firstAvailable_ = newYbn->next;

	return newYbn;
}

void YbnPool::Remove(YbnLoader * ybn)
{
	ybn->Remove();
	ybn->next = firstAvailable_;
	firstAvailable_ = ybn;
}
