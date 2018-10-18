#include "YbnLoader.h"

void YbnLoader::Init(memstream2& file)
{
	compound = new btCompoundShape();

	ParseYbn(file);
}

void YbnLoader::ParseYbn(memstream2 & file)
{
	Bounds* bounds = (Bounds*)file.read(sizeof(Bounds));

	switch (bounds->Type)
	{
		case 0: {
			btSphereShape* sphere = new btSphereShape(0.5f);
			Shapes.push_back(sphere);

			btTransform localTrans;
			localTrans.setIdentity();
			compound->addChildShape(localTrans, sphere);

			break;
		}
		case 1: {
			btCapsuleShapeZ* capsule = new btCapsuleShapeZ(0.5f, 0.5f);
			Shapes.push_back(capsule);

			btTransform localTrans;
			localTrans.setIdentity();
			compound->addChildShape(localTrans, capsule);

			break;
		}
		case 3: {
			btBoxShape* box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
			Shapes.push_back(box);

			btTransform localTrans;
			localTrans.setIdentity();

			compound->addChildShape(localTrans, box);

			break;
		}
		case 4: case 8: {
			BoundGeometry* geom = (BoundGeometry*)file.read(sizeof(BoundGeometry));

			///////////////////
			SYSTEM_BASE_PTR(geom->VerticesPointer);
			file.seekg(geom->VerticesPointer);

			glm::i16vec3 *CompressedVertices = (glm::i16vec3*)file.read(sizeof(glm::i16vec3));

			glm::vec3* Vertices = myNew glm::vec3[geom->VerticesCount];
			VerticesArray.push_back(Vertices);

			for (uint32_t i = 0; i < geom->VerticesCount; i++)
			{
				Vertices[i] = glm::vec3(CompressedVertices[i].x, CompressedVertices[i].y, CompressedVertices[i].z) * geom->Quantum;
			}

			///////////////////

			std::vector<BoundPolygonTriangle*> PolygonTriangles;

			SYSTEM_BASE_PTR(geom->PolygonsPointer);
			file.seekg(geom->PolygonsPointer);

			Shapes.reserve(geom->PolygonsCount);

			for (uint32_t i = 0; i < geom->PolygonsCount; i++)  //PERFORMANCE IMPROVEMENT???
			{
				uint8_t* type = (uint8_t*)file.read(sizeof(uint8_t));
				file.seekCur(-1);

				switch (type[0] & 7)
				{
					case 0: {
						BoundPolygonTriangle* PolygonTriangle = (BoundPolygonTriangle*)file.read(sizeof(BoundPolygonTriangle));
						PolygonTriangles.push_back(PolygonTriangle);
						break;
					}
					case 1: {
						BoundPolygonSphere* PolygonSphere = (BoundPolygonSphere*)file.read(sizeof(BoundPolygonSphere));

						btSphereShape* sphere = new btSphereShape(PolygonSphere->sphereRadius);
						Shapes.push_back(sphere);

						btTransform localTrans;
						localTrans.setIdentity();
						localTrans.setOrigin(btVector3(geom->CenterGeom.x + Vertices[PolygonSphere->sphereIndex].x, geom->CenterGeom.y + Vertices[PolygonSphere->sphereIndex].y, geom->CenterGeom.z + Vertices[PolygonSphere->sphereIndex].z));
						compound->addChildShape(localTrans, sphere);
						break;
					}
					case 2: {
						BoundPolygonCapsule* PolygonCapsule = (BoundPolygonCapsule*)file.read(sizeof(BoundPolygonCapsule));

						auto mid = (Vertices[PolygonCapsule->capsuleIndex1] + Vertices[PolygonCapsule->capsuleIndex2]) / 2.f;

						btCapsuleShapeZ* capsule = new btCapsuleShapeZ(PolygonCapsule->capsuleRadius, 0.5f);
						Shapes.push_back(capsule);

						btTransform localTrans;
						localTrans.setIdentity();
						localTrans.setOrigin(btVector3(geom->CenterGeom.x + mid.x, geom->CenterGeom.y + mid.y, geom->CenterGeom.z + mid.z));
						compound->addChildShape(localTrans, capsule);
						break;
					}
					case 3: {
						BoundPolygonBox* PolygonBox = (BoundPolygonBox*)file.read(sizeof(BoundPolygonBox));

						/*if (PolygonBoxes.size() != 0) {
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

				btBoxShape* shape = myNew btBoxShape(btVector3(-size.x, -size.y, size.z));

				Shapes.push_back(shape);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom->CenterGeom.x + mid.x, geom->CenterGeom.y + mid.y, geom->CenterGeom.z + mid.z));
				compound->addChildShape(localTrans, shape);
				}
				}*/
						break;
					}
					case 4: {
						BoundPolygonCylinder* PolygonCylinder = (BoundPolygonCylinder*)file.read(sizeof(BoundPolygonCylinder));

						btCylinderShapeZ* shape = new btCylinderShapeZ(btVector3(0.5, 0.5, 0.5));
						Shapes.push_back(shape);

						btTransform localTrans;
						localTrans.setIdentity();
						localTrans.setOrigin(btVector3(geom->CenterGeom.x + Vertices[PolygonCylinder->cylinderIndex1].x, geom->CenterGeom.y + Vertices[PolygonCylinder->cylinderIndex1].y, geom->CenterGeom.z + Vertices[PolygonCylinder->cylinderIndex1].z));
						compound->addChildShape(localTrans, shape);
						break;
					}
					default:
						file.seekCur(16);
						//printf("ERROR NOT IMPLEMENTED!");
						break;
				}
			}

			if (PolygonTriangles.size() != 0) {
				glm::u16vec3* Indices = new glm::u16vec3[PolygonTriangles.size()];
				IndicesArray.push_back(Indices);

				for (int i = 0; i < PolygonTriangles.size(); i++)
				{
					Indices[i] = glm::u16vec3(PolygonTriangles[i]->triIndex1 & 0x7FFF, PolygonTriangles[i]->triIndex2 & 0x7FFF, PolygonTriangles[i]->triIndex3 & 0x7FFF);
				}

				/*btQuantizedBvh* quantizedBvh = myNew btQuantizedBvh();
				quantizedBvh->setQuantizationValues(btVector3(Bounds.BoundingBoxMin.x, Bounds.BoundingBoxMin.y, Bounds.BoundingBoxMin.z), btVector3(Bounds.BoundingBoxMax.x, Bounds.BoundingBoxMax.y, Bounds.BoundingBoxMax.z));
				QuantizedNodeArray&	nodes = quantizedBvh->getLeafNodeArray();
				btOptimizedBvh *bvh = myNew btOptimizedBvh();
				//bvh->build(VertIndicesArray, true, );
				trishape->setOptimizedBvh(bvh);
				btQuantizedBvhNode node;*/

				btIndexedMesh mesh;
				mesh.m_numTriangles = PolygonTriangles.size();
				mesh.m_triangleIndexBase = (uint8_t*)&Indices[0];
				mesh.m_triangleIndexStride = 3 * sizeof(uint16_t);
				mesh.m_numVertices = geom->VerticesCount;
				mesh.m_vertexBase = (uint8_t*)&Vertices[0];
				mesh.m_vertexStride = sizeof(glm::vec3);

				btTriangleIndexVertexArray* VertIndices = new btTriangleIndexVertexArray();
				VertIndicesArray.push_back(VertIndices);
				VertIndices->addIndexedMesh(mesh, PHY_SHORT);

				btBvhTriangleMeshShape * trishape = new btBvhTriangleMeshShape(VertIndices, false);
				trishapes.push_back(trishape);
				trishape->setMargin(bounds->Margin);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom->CenterGeom.x, geom->CenterGeom.y, geom->CenterGeom.z));
				compound->addChildShape(localTrans, trishape);
			}

			break;
		}
		case 10: {
			BoundComposite* boundComposite = (BoundComposite*)file.read(sizeof(BoundComposite));

			SYSTEM_BASE_PTR(boundComposite->ChildrenPointer);
			file.seekg(boundComposite->ChildrenPointer);

			for (int i = 0; i < boundComposite->ChildrenCount1; i++)
			{
				uint64_t* DataPointer = (uint64_t*)file.read(sizeof(uint64_t));

				uint64_t BoundsPointer = file.tellg();

				SYSTEM_BASE_PTR(DataPointer[0]);
				file.seekg(DataPointer[0]);

				ParseYbn(file);

				file.seekg(BoundsPointer);
			}

			break;
		}
		case 13: {
			btCylinderShapeZ* shape = new btCylinderShapeZ(btVector3(0.5, 0.5, 0.5));
			Shapes.push_back(shape);

			btTransform localTrans;
			localTrans.setIdentity();
			compound->addChildShape(localTrans, shape);

			break;
		}
		default:
			printf("WTF");
			break;
	}
}

void YbnLoader::Finalize(btDiscreteDynamicsWorld* world)
{
	btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, MotionState, compound, btVector3(0, 0, 0));
	rigidBody = new btRigidBody(groundRigidBodyCI);
	world->addRigidBody(rigidBody);

	Loaded = true;
}

void YbnLoader::Remove()
{
	for (auto& shape : Shapes)
	{
		delete shape;
	}

	Shapes.clear();

	for (auto & shape : trishapes)
	{
		delete shape;
	}

	for (auto & Array : VertIndicesArray)
	{
		delete Array;
	}

	for (auto & Indices : IndicesArray)
	{
		delete Indices;
	}

	for (auto & Vertices : VerticesArray)
	{
		delete Vertices;
	}
	IndicesArray.clear();
	VerticesArray.clear();
	trishapes.clear();
	VertIndicesArray.clear();

	if (compound) {
		delete compound;
		compound = nullptr;
	}
}
