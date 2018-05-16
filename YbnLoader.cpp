#include "YbnLoader.h"

YbnLoader::YbnLoader()
{

}

YbnLoader::YbnLoader(btDiscreteDynamicsWorld * world, memstream & file)
{
	Init(world, file);
}

void YbnLoader::Init(btDiscreteDynamicsWorld* world, memstream& file)
{
	CollisionWorld = world;

	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	file.read((char*)&Bounds, sizeof(Bounds));

	if (Bounds.Type == 8 || Bounds.Type == 4) {
		btCompoundShape* compound = new btCompoundShape();
		Shapes.push_back(compound);

		BoundGeometry geom;
		file.read((char*)&geom, sizeof(BoundGeometry));

		indices.reserve(geom.PolygonsCount * 3);

		std::vector<BoundPolygonSphere> PolygonSpheres;
		std::vector<BoundPolygonCapsule> PolygonCapsules;
		std::vector<BoundPolygonBox> PolygonBoxes;
		std::vector<BoundPolygonCylinder> PolygonCylinders;

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
				PolygonCapsules.push_back(PolygonCapsule);
				break;
			case 3:
				BoundPolygonBox PolygonBox;
				file.read((char*)&PolygonBox, sizeof(BoundPolygonBox));
				PolygonBoxes.push_back(PolygonBox);
				break;
			case 4:
				BoundPolygonCylinder PolygonCylinder;
				file.read((char*)&PolygonCylinder, sizeof(BoundPolygonCylinder));
				PolygonCylinders.push_back(PolygonCylinder);
				break;
			default:
				file.seekg(16, std::ios::cur);
				//printf("ERROR NOT IMPLEMENTED!");
				break;
			}
		}


		///////////////////
		TranslatePTR(geom.VerticesPointer);

		file.seekg(geom.VerticesPointer);

		glm::i16vec3 *vertices = new glm::i16vec3[geom.VerticesCount];
		file.read((char*)&vertices[0], sizeof(glm::i16vec3) * geom.VerticesCount);

		Vertices.resize(geom.VerticesCount);

		for (uint32_t i = 0; i < geom.VerticesCount; i++)
		{
			Vertices[i] = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z) * geom.Quantum;
		}

		delete[] vertices;

		if (PolygonCylinders.size() != 0) {
			for (int i = 0; i < PolygonCylinders.size(); i++)
			{
				btCylinderShapeZ* shape = new btCylinderShapeZ(btVector3(0.5, 0.5, 0.5));
				Shapes.push_back(shape);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom.CenterGeom.x + Vertices[PolygonCylinders[i].cylinderIndex1].x, geom.CenterGeom.y + Vertices[PolygonCylinders[i].cylinderIndex1].y, geom.CenterGeom.z + Vertices[PolygonCylinders[i].cylinderIndex1].z));
				compound->addChildShape(localTrans, shape);
			}
		}

		if (PolygonBoxes.size() != 0) {
			for (int i = 0; i < PolygonBoxes.size(); i++)
			{

				glm::vec3 p1 = Vertices[PolygonBoxes[i].boxIndex1];
				glm::vec3 p2 = Vertices[PolygonBoxes[i].boxIndex2];
				glm::vec3 p3 = Vertices[PolygonBoxes[i].boxIndex3];
				glm::vec3 p4 = Vertices[PolygonBoxes[i].boxIndex4];

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
				localTrans.setOrigin(btVector3(geom.CenterGeom.x + mid.x, geom.CenterGeom.y + mid.y, geom.CenterGeom.z + mid.z));
				compound->addChildShape(localTrans, shape);
			}
		}

		if (PolygonCapsules.size() != 0) {
			for (int i = 0; i < PolygonCapsules.size(); i++) //Height is incorrect
			{
				auto mid = (Vertices[PolygonCapsules[i].capsuleIndex1] + Vertices[PolygonCapsules[i].capsuleIndex2]) / 2.f;

				btCapsuleShapeZ* capsule = new btCapsuleShapeZ(PolygonCapsules[i].capsuleRadius, 1.0f);
				Shapes.push_back(capsule);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom.CenterGeom.x + mid.x, geom.CenterGeom.y + mid.y, geom.CenterGeom.z + mid.z));
				compound->addChildShape(localTrans, capsule);
			}
		}

		if (PolygonSpheres.size() != 0) {
			for (int i = 0; i < PolygonSpheres.size(); i++) //works as expected
			{
				btSphereShape* sphere = new btSphereShape(PolygonSpheres[i].sphereRadius);
				Shapes.push_back(sphere);

				btTransform localTrans;
				localTrans.setIdentity();
				localTrans.setOrigin(btVector3(geom.CenterGeom.x + Vertices[PolygonSpheres[i].sphereIndex].x, geom.CenterGeom.y + Vertices[PolygonSpheres[i].sphereIndex].y, geom.CenterGeom.z + Vertices[PolygonSpheres[i].sphereIndex].z));
				compound->addChildShape(localTrans, sphere);
			}
		}

		if (indices.size() != 0) {
			VertIndicesArray = new btTriangleIndexVertexArray(indices.size() / 3, (int*)indices.data(), 3 * sizeof(int32_t),
				Vertices.size(), &Vertices[0][0], sizeof(glm::vec3));

			trishape = new btBvhTriangleMeshShape(VertIndicesArray, false);
			trishape->setMargin(Bounds.Margin);

			btTransform localTrans;
			localTrans.setIdentity();
			localTrans.setOrigin(btVector3(geom.CenterGeom.x, geom.CenterGeom.y, geom.CenterGeom.z));
			compound->addChildShape(localTrans, trishape);
		}

		btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(0, 0, 0)));
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, MotionState, compound, btVector3(0, 0, 0));
		rigidBody = new btRigidBody(groundRigidBodyCI);
		world->addRigidBody(rigidBody);
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

	for (auto& shape : Shapes)
	{
		delete shape;
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
}
