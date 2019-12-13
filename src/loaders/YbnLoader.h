#pragma once
#include <vector>
#include "FileType.h"
#include <btBulletDynamicsCommon.h>

enum class phBoundType : uint8_t {
    Sphere = 0,
    Capsule = 1,
    Box = 3,
    Geometry = 4,
    BVH = 8,
    Composite = 10
};

struct phBoundMaterial1
{
    uint8_t materialIdx;
    uint8_t proceduralId;

        // TODO: double-check order
        uint8_t roomId : 5;
        uint8_t pedDensity : 3;

        uint8_t stairs : 1;
        uint8_t blockClimb : 1;
        uint8_t seeThrough : 1;
        uint8_t shootThrough : 1;
        uint8_t notCover : 1;
        uint8_t walkablePath : 1;
        uint8_t noCamCollision : 1;
        uint8_t shootThroughFx : 1;
};

struct phBoundMaterial2
{
    uint8_t noDecal : 1;
    uint8_t noNavmesh : 1;
    uint8_t noRagdoll : 1;
    uint8_t vehicleWheel : 1;
    uint8_t noPtfx : 1;
    uint8_t tooSteepForPlayer : 1;
    uint8_t noNetworkSpawn : 1;
    uint8_t noCamCollisionAllowClipping : 1;
    uint8_t materialColorIdx;
    uint16_t unknown;
};

struct phBoundMaterial
{
    phBoundMaterial1 mat1;
    phBoundMaterial2 mat2;
};

struct phBound : pgBase
{
    phBoundType boundType;
    uint8_t Unknown_11h;
    uint16_t Unknown_12h;
    float BoundingSphereRadius;
    uint64_t pad;
    glm::vec3 BoundingBoxMax;
    float Margin;
    glm::vec3 BoundingBoxMin;
    uint32_t Unknown_3Ch;
    glm::vec3 BoundingBoxCenter;
    phBoundMaterial1 material;
    glm::vec3 Center;
    phBoundMaterial2 material2;
    float UnknownVector3[3];
    float BoundingBoxVolume;
};

enum class BoundPolygonType : uint32_t
{
    Triangle = 0,
    Sphere = 1,
    Capsule = 2,
    Box = 3,
    Cylinder = 4,
};

struct phBoundPoly
{
public:
    union
    {
        struct
        {
            BoundPolygonType type : 3; // 0: triangle, 1: sphere, 2: capsule, 3: box, 4: cylinder
        };

        struct
        {
            uint16_t type; // 1
            uint16_t index;

            float radius;
        } sphere;

        struct
        {
            uint16_t type; // 2
            uint16_t index;

            float length;
            int16_t indexB;
        } capsule;

        struct
        {
            uint32_t type; // 3

            int16_t indices[4];
        } box;

        struct
        {
            float triangleArea;

            int16_t v1;
            int16_t v2;
            int16_t v3;
            int16_t e1;
            int16_t e2;
            int16_t e3;
        } poly;
    };
};

struct phBoundPolyhedron : public phBound
{
    uint32_t Unknown_70h;
    uint32_t Unknown_74h;
    uint64_t Unknown_78h_Pointer;
    uint32_t Unknown_80h;
    uint32_t Count1;
    pgPtr<phBoundPoly> polygons;
    glm::vec3 Quantum;
    float Unknown_9Ch;
    glm::vec3 CenterGeom;
    float Unknown_ACh;
    pgPtr<int16_t> vertices;
    uint64_t Unknown_B8h_Pointer;
    uint64_t Unknown_C0h_Pointer;
    uint64_t Unknown_C8h_Pointer;
    uint32_t VerticesCount;
    uint32_t PolygonsCount;
    uint64_t Unknown_D8h;  // 0x00000000
    uint64_t Unknown_DCh;  // 0x00000000
    uint64_t Unknown_E0h;  // 0x00000000
    
    int16_t* getVertices() {
        return *vertices;
    }
    
    phBoundPoly* getPolygons() {
        return *polygons;
    }
    
    void Resolve(memstream& file)
    {
        polygons.Resolve(file);
        
        vertices.Resolve(file);
    }
};

struct phBoundGeometry : public phBoundPolyhedron
{
    pgPtr<phBoundMaterial> materials;
    pgPtr<uint32_t> materialColours;
    uint32_t Unknown_100h;  // 0x00000000
    uint32_t Unknown_104h;  // 0x00000000
    uint32_t Unknown_108h;  // 0x00000000
    uint32_t Unknown_10Ch;  // 0x00000000
    uint32_t Unknown_110h;  // 0x00000000
    uint32_t Unknown_114h;  // 0x00000000
    uint64_t PolygonMaterialIndicesPointer;
    uint8_t MaterialsCount;
    uint8_t MaterialColoursCount;
    uint16_t Unknown_122h;  // 0x0000
    uint32_t Unknown_124h;  // 0x00000000
    uint32_t Unknown_128h;  // 0x00000000
    uint32_t Unknown_12Ch;  // 0x00000000
    
    void Resolve(memstream& file)
    {
        phBoundPolyhedron::Resolve(file);
    }
};

struct BoundPolygonTriangle
{
    float triArea;
    uint16_t triIndex1;
    uint16_t triIndex2;
    uint16_t triIndex3;
    int16_t edgeIndex1;
    int16_t edgeIndex2;
    int16_t edgeIndex3;
};

struct BoundPolygonSphere
{
    uint16_t sphereType;
    uint16_t sphereIndex;
    float sphereRadius;
    uint32_t unused0;
    uint32_t unused1;
};

struct phBoundCapsule
{
    uint16_t capsuleType;
    uint16_t capsuleIndex1;
    float capsuleRadius;
    uint16_t capsuleIndex2;
    uint16_t unused0;
    uint32_t unused1;
};

struct BoundPolygonBox
{
    uint32_t boxType;
    int16_t boxIndex1;
    int16_t boxIndex2;
    int16_t boxIndex3;
    int16_t boxIndex4;
    uint32_t unused0;
};

struct BoundPolygonCylinder
{
    uint16_t cylinderType;
    uint16_t cylinderIndex1;
    float cylinderRadius;
    uint16_t cylinderIndex2;
    uint16_t unused0;
    uint32_t unused1;
};

struct phBoundFlagEntry
{
    uint32_t m_0; // boundflags value?
    uint32_t m_4; // defaults to -1 during import, though other values are also seen
};

class phBoundComposite : phBound
{
    pgPtr<pgPtr<phBound>> childrens;
    uint64_t ChildrenTransformation1Pointer;
    uint64_t ChildrenTransformation2Pointer;
    uint64_t ChildrenBoundingBoxesPointer;
    pgPtr<phBoundFlagEntry> m_boundFlags;
    pgArray<phBoundFlagEntry> m_childArray;
    uint64_t BVHPointer;
    
public:
    void Resolve(memstream& file)
    {
        //phBound::Resolve(file);
        
        childrens.Resolve(file);
        
        m_boundFlags.Resolve(file);
        
        for (int i = 0; i < m_childArray.GetCount(); i++) {
            (*childrens)[i].Resolve(file);
        }
    }
    
    inline uint16_t getNumChildBounds()
    {
        return m_childArray.GetSize();
    }
    
    inline phBound* getChildBound(uint16_t index)
    {
        return *((*childrens)[index]);
    }
};

class YbnLoader : public FileType
{
    std::vector<glm::u16vec3*> IndicesArray;
    std::vector<glm::vec3*> VerticesArray;
    std::unique_ptr<btRigidBody> rigidBody;
    
    void addBoxShape(btCompoundShape* compound, btVector3 pos, btVector3 halfExtents);
    void addCapsuleShape(btCompoundShape* compound, btVector3 pos, float radius, float height);
    void addSphereShape(btCompoundShape* compound, btVector3 pos, float SphereRadius);
    void addCylinderShape(btCompoundShape* compound, btVector3 pos, btVector3 halfExtents);
    void parseYbn(memstream& file, phBound* bound);
public:
    btCompoundShape* compound;
    btRigidBody* getRigidBody() const
    {
        return rigidBody.get();
    }

    void finalize(BaseRenderer* _renderer, phBound* boundData, memstream& file);
    ~YbnLoader();
};
