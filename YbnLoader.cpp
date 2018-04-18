#include "YbnLoader.h"

YbnLoader::YbnLoader(btDiscreteDynamicsWorld* world, memstream& file) : CollisionWorld(world)
{
	compound = new btCompoundShape();

	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	file.read((char*)&Bounds, sizeof(Bounds));

	if (Bounds.Type == 8 || Bounds.Type == 4) {
		BoundGeometry geom;
		file.read((char*)&geom, sizeof(BoundGeometry));

		indices.reserve(geom.PolygonsCount * 3);

		std::vector<BoundPolygonSphere> PolygonSpheres;

		TranslatePTR(geom.PolygonsPointer);

		file.seekg(geom.PolygonsPointer);

		for (uint32_t i = 0; i < geom.PolygonsCount; i++)  //PERFORMANCE IMPROVEMENT???
		{
			uint8_t type;
			file.read((char*)&type, sizeof(uint8_t));
			file.seekg(-1, std::ios::cur);

			switch (type & 7)
			{
			case 0:
				BoundPolygonTriangle PolygonTriangle;
				file.read((char*)&PolygonTriangle, sizeof(BoundPolygonTriangle));
				indices.push_back(PolygonTriangle.triIndex1 & 0x7FFF);
				indices.push_back(PolygonTriangle.triIndex2 & 0x7FFF);
				indices.push_back(PolygonTriangle.triIndex3 & 0x7FFF);
				break;
			case 1:
				BoundPolygonSphere PolygonSphere;
				file.read((char*)&PolygonSphere, sizeof(BoundPolygonSphere));
				PolygonSpheres.push_back(PolygonSphere);
				break;
			case 2:
				BoundPolygonCapsule PolygonCapsule;
				file.read((char*)&PolygonCapsule, sizeof(BoundPolygonCapsule));
				break;
			case 3:
				BoundPolygonBox PolygonBox;
				file.read((char*)&PolygonBox, sizeof(BoundPolygonBox));
				break;
			case 4:
				BoundPolygonCylinder PolygonCylinder;
				file.read((char*)&PolygonCylinder, sizeof(BoundPolygonCylinder));
				break;
			default:
				file.seekg(16, std::ios::cur);
				//printf("ERROR NOT IMPLEMENTED!");
				break;
			}
		}

		///////////////////
		if (indices.size() != 0) {
			TranslatePTR(geom.VerticesPointer);

			file.seekg(geom.VerticesPointer);

			Vertex_HalfType *vertices = new Vertex_HalfType[geom.VerticesCount];
			file.read((char*)&vertices[0], sizeof(Vertex_HalfType) * geom.VerticesCount);

			Vertices.resize(geom.VerticesCount);

			for (uint32_t i = 0; i < geom.VerticesCount; i++)
			{
				Vertices[i] = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z) * geom.Quantum;
			}

			delete[] vertices;

			/*if (PolygonSpheres.size() > 0) {
				for (int i = 0; i < PolygonSpheres.size(); i++)
				{
					btSphereShape* sphere = new btSphereShape(PolygonSpheres[i].sphereRadius);

					btTransform localTrans;
					localTrans.setIdentity();
					//localTrans effectively shifts the center of mass with respect to the chassis
					localTrans.setOrigin(btVector3(geom.CenterGeom.x * Vertices[PolygonSpheres[i].sphereIndex].x, geom.CenterGeom.y* Vertices[PolygonSpheres[i].sphereIndex].y, geom.CenterGeom.z* Vertices[PolygonSpheres[i].sphereIndex].z));
					compound->addChildShape(localTrans, sphere);
				}
			}
			else {*/
				VertIndicesArray = new btTriangleIndexVertexArray(indices.size() /3 , (int*)indices.data(), 3 * sizeof(int32_t),
					Vertices.size(), &Vertices[0][0], sizeof(glm::vec3));

				trishape = new btBvhTriangleMeshShape(VertIndicesArray, false);
				trishape->setMargin(Bounds.Margin);

				btTransform localTrans;
				localTrans.setIdentity();
				//localTrans effectively shifts the center of mass with respect to the chassis
				localTrans.setOrigin(btVector3(geom.CenterGeom.x, geom.CenterGeom.y, geom.CenterGeom.z));
				compound->addChildShape(localTrans, trishape);
			//}

			btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0, 0, 0)));
			btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, MotionState, compound, btVector3(0, 0, 0));
			rigidBody = new btRigidBody(groundRigidBodyCI);
			world->addRigidBody(rigidBody);
		}
	}

	if (Bounds.Type == 10) {

		file.read((char*)&BoundComposite, sizeof(BoundComposite));

		TranslatePTR(BoundComposite.ChildrenPointer);

		file.seekg(BoundComposite.ChildrenPointer);
		ybns.reserve(BoundComposite.ChildrenCount1);

		for (int i = 0; i < BoundComposite.ChildrenCount1; i++)
		{
			uint64_t DataPointer;
			file.read((char*)&DataPointer, sizeof(uint64_t));

			uint64_t BoundsPointer = file.tellg();

			TranslatePTR(DataPointer);

			file.seekg(DataPointer);

			YbnLoader* ybn = new YbnLoader(world, file);
			ybns.push_back(ybn);

			file.seekg(BoundsPointer);
		}
	}
}


YbnLoader::~YbnLoader()
{
	for (int i = 0; i < ybns.size(); i++)
	{
		delete ybns[i];
	}

	if (VertIndicesArray)
		delete VertIndicesArray;
	if (trishape)
		delete trishape;
	if (rigidBody) {
		delete rigidBody->getMotionState();

		CollisionWorld->removeRigidBody(rigidBody);

		delete rigidBody;
	}
	delete compound;
}
