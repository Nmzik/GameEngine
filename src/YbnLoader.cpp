#include "YbnLoader.h"
#include "PhysicsSystem.h"

void YbnLoader::addBoxShape(btCompoundShape* compound, btVector3 pos, btVector3 halfExtents)
{
    btTransform localTrans;
    localTrans.setIdentity();
    localTrans.setOrigin(pos);
    compound->addChildShape(localTrans, new btBoxShape(halfExtents));
}

void YbnLoader::addCapsuleShape(btCompoundShape* compound, btVector3 pos, float radius, float height)
{
    btTransform localTrans;
    localTrans.setIdentity();
    localTrans.setOrigin(pos);
    compound->addChildShape(localTrans, new btCapsuleShapeZ(radius, height));
}

void YbnLoader::addSphereShape(btCompoundShape* compound, btVector3 pos, float SphereRadius)
{
    btTransform localTrans;
    localTrans.setIdentity();
    localTrans.setOrigin(pos);
    compound->addChildShape(localTrans, new btSphereShape(SphereRadius));
}

void YbnLoader::addCylinderShape(btCompoundShape* compound, btVector3 pos, btVector3 halfExtents)
{
    btTransform localTrans;
    localTrans.setIdentity();
    localTrans.setOrigin(pos);
    compound->addChildShape(localTrans, new btCylinderShapeZ(halfExtents));
}

void YbnLoader::Init(memstream& file)
{
    Loaded = true;
    btCompoundShape* compound = new btCompoundShape();

    ParseYbn(file, compound);

    btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(0.f, 0.f, 0.f, -1.f), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, MotionState, compound, btVector3(0, 0, 0));
    rigidBody = std::make_unique<btRigidBody>(groundRigidBodyCI);
}

void YbnLoader::ParseYbn(memstream& file, btCompoundShape* compound)
{
    Bounds* bounds = (Bounds*)file.read(sizeof(Bounds));

    switch (bounds->Type)
    {
        case 0:
        {
            addSphereShape(compound, btVector3(0, 0, 0), 0.5f);
            break;
        }
        case 1:
        {
            addCapsuleShape(compound, btVector3(0, 0, 0), 0.5f, 0.5f);
            break;
        }
        case 3:
        {
            addBoxShape(compound, btVector3(0, 0, 0), btVector3(0.5f, 0.5f, 0.5f));
            break;
        }
        case 4:
        case 8:
        {
            BoundGeometry* geom = (BoundGeometry*)file.read(sizeof(BoundGeometry));

            ///////////////////
            SYSTEM_BASE_PTR(geom->VerticesPointer);
            file.seekg(geom->VerticesPointer);

            glm::i16vec3* CompressedVertices = (glm::i16vec3*)file.read(sizeof(glm::i16vec3));

            glm::vec3* Vertices = (glm::vec3*)btAlignedAllocInternal(geom->VerticesCount * sizeof(glm::vec3), 16);
            VerticesArray.push_back(Vertices);

            for (uint32_t i = 0; i < geom->VerticesCount; i++)
            {
                Vertices[i] = glm::vec3(CompressedVertices[i].x, CompressedVertices[i].y, CompressedVertices[i].z) * geom->Quantum;
            }

            ///////////////////

            std::vector<BoundPolygonTriangle*> PolygonTriangles;

            SYSTEM_BASE_PTR(geom->PolygonsPointer);
            file.seekg(geom->PolygonsPointer);

            for (uint32_t i = 0; i < geom->PolygonsCount; i++)  //	PERFORMANCE IMPROVEMENT???
            {
                uint8_t type = *(uint8_t*)file.read(sizeof(uint8_t));
                file.seekCur(-1);

                switch (type & 7)
                {
                    case 0:
                    {
                        BoundPolygonTriangle* PolygonTriangle = (BoundPolygonTriangle*)file.read(sizeof(BoundPolygonTriangle));
                        PolygonTriangles.push_back(PolygonTriangle);
                        break;
                    }
                    case 1:
                    {
                        BoundPolygonSphere* PolygonSphere = (BoundPolygonSphere*)file.read(sizeof(BoundPolygonSphere));
                        addSphereShape(compound, btVector3(geom->CenterGeom.x + Vertices[PolygonSphere->sphereIndex].x, geom->CenterGeom.y + Vertices[PolygonSphere->sphereIndex].y, geom->CenterGeom.z + Vertices[PolygonSphere->sphereIndex].z),
                                       PolygonSphere->sphereRadius);
                        break;
                    }
                    case 2:
                    {
                        //	USED FOR BUSHES - NOT FOR ACTUAL COLLISION DETECTION!
                        BoundPolygonCapsule* PolygonCapsule = (BoundPolygonCapsule*)file.read(sizeof(BoundPolygonCapsule));
                        //auto mid = (Vertices[PolygonCapsule->capsuleIndex1] + Vertices[PolygonCapsule->capsuleIndex2]) / 2.f;
                        //addCapsuleShape(compound, btVector3(geom->CenterGeom.x + mid.x, geom->CenterGeom.y + mid.y, geom->CenterGeom.z + mid.z),
                        //  PolygonCapsule->capsuleRadius, 0.5f);
                        break;
                    }
                    case 3:
                    {
                        BoundPolygonBox* PolygonBox = (BoundPolygonBox*)file.read(sizeof(BoundPolygonBox));

                        glm::vec3 p1 = Vertices[PolygonBox->boxIndex1];
                        glm::vec3 p2 = Vertices[PolygonBox->boxIndex2];
                        glm::vec3 p3 = Vertices[PolygonBox->boxIndex3];
                        glm::vec3 p4 = Vertices[PolygonBox->boxIndex4];

                        glm::vec3 p1p2max = glm::max(p1, p2);
                        glm::vec3 p3p4max = glm::max(p3, p4);
                        glm::vec3 max = glm::max(p1p2max, p3p4max);

                        glm::vec3 p1p2min = glm::min(p1, p2);
                        glm::vec3 p3p4min = glm::min(p3, p4);
                        glm::vec3 min = glm::min(p1p2min, p3p4min);

                        glm::vec3 size = ((p3 + p4) - (p1 + p2)) * 0.5f;  //	Half extents
                        auto mid = (min + max) / 2.f;

                        addBoxShape(compound,
                                    btVector3(geom->CenterGeom.x + mid.x, geom->CenterGeom.y + mid.y, geom->CenterGeom.z + mid.z),
                                    btVector3(size.x, size.y, size.z));
                        break;
                    }
                    case 4:
                    {
                        BoundPolygonCylinder* PolygonCylinder = (BoundPolygonCylinder*)file.read(sizeof(BoundPolygonCylinder));
                        addCylinderShape(compound, btVector3(geom->CenterGeom.x + Vertices[PolygonCylinder->cylinderIndex1].x, geom->CenterGeom.y + Vertices[PolygonCylinder->cylinderIndex1].y, geom->CenterGeom.z + Vertices[PolygonCylinder->cylinderIndex1].z),
                                         btVector3(0.5, 0.5, 0.5));
                        break;
                    }
                    default:
                        assert("UNIMPLEMENTED COL SHAPE");
                        file.seekCur(16);
                        //	printf("ERROR NOT IMPLEMENTED!");
                        break;
                }
            }

            if (PolygonTriangles.size() != 0)
            {
                glm::u16vec3* Indices = (glm::u16vec3*)btAlignedAllocInternal(PolygonTriangles.size() * sizeof(glm::u16vec3), 16);
                IndicesArray.push_back(Indices);

                for (int i = 0; i < PolygonTriangles.size(); i++)
                {
                    Indices[i] =
                        glm::u16vec3(PolygonTriangles[i]->triIndex1 & 0x7FFF, PolygonTriangles[i]->triIndex2 & 0x7FFF, PolygonTriangles[i]->triIndex3 & 0x7FFF);
                }

                btIndexedMesh mesh;
                mesh.m_numTriangles = (int)PolygonTriangles.size();
                mesh.m_triangleIndexBase = (uint8_t*)&Indices[0];
                mesh.m_triangleIndexStride = 3 * sizeof(uint16_t);
                mesh.m_numVertices = geom->VerticesCount;
                mesh.m_vertexBase = (uint8_t*)&Vertices[0];
                mesh.m_vertexStride = sizeof(glm::vec3);

                btTriangleIndexVertexArray* VertIndices = new btTriangleIndexVertexArray();
                VertIndices->addIndexedMesh(mesh, PHY_SHORT);

                btTransform localTrans;
                localTrans.setIdentity();
                localTrans.setOrigin(btVector3(geom->CenterGeom.x, geom->CenterGeom.y, geom->CenterGeom.z));

                btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(VertIndices, true);
                shape->setMargin(bounds->Margin);
                compound->addChildShape(localTrans, shape);
            }

            break;
        }
        case 10:
        {
            BoundComposite* boundComposite = (BoundComposite*)file.read(sizeof(BoundComposite));

            SYSTEM_BASE_PTR(boundComposite->ChildrenPointer);
            file.seekg(boundComposite->ChildrenPointer);

            for (int i = 0; i < boundComposite->ChildrenCount1; i++)
            {
                uint64_t DataPointer = *(uint64_t*)file.read(sizeof(uint64_t));

                uint64_t BoundsPointer = file.tellg();

                SYSTEM_BASE_PTR(DataPointer);
                file.seekg(DataPointer);

                ParseYbn(file, compound);

                file.seekg(BoundsPointer);
            }

            break;
        }
        case 13:
        {
            addCylinderShape(compound, btVector3(0, 0, 0), btVector3(0.5, 0.5, 0.5));
            break;
        }
        default:
            break;
            //	12 ???
    }
}

YbnLoader::~YbnLoader()
{
    if (rigidBody)
    {
        btCompoundShape* compound = static_cast<btCompoundShape*>(rigidBody->getCollisionShape());

        int numChild = compound->getNumChildShapes();
        for (int i = 0; i < numChild; i++)
        {
            btCollisionShape* shape = compound->getChildShape(i);
            if (shape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
                delete static_cast<btBvhTriangleMeshShape*>(shape)->getMeshInterface();
            delete shape;
        }

        for (auto& Vertices : VerticesArray)
        {
            btAlignedFreeInternal(Vertices);
        }

        for (auto& Indices : IndicesArray)
        {
            btAlignedFreeInternal(Indices);
        }

        delete rigidBody->getMotionState();
        delete compound;
    }
}
