#pragma once
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "YdrLoader.h"
#include <Recast.h>

class YnvLoader
{
    enum NavMeshFlags
    {
        None = 0,
        Vertices = 1,
        Portals = 2,
        Vehicle = 4,
        Unknown8 = 8,
    };

    struct NavMeshUintArray
    {
        uint32_t Count;
        uint32_t v00;
        uint32_t v01;
        uint32_t v02;
        uint32_t v03;
        uint32_t v04;
        uint32_t v05;
        uint32_t v06;  // 0x00000000
        uint32_t v07;  // 0x00000000
        uint32_t v08;  // 0x00000000
        uint32_t v09;  // 0x00000000
        uint32_t v10;  // 0x00000000
        uint32_t v11;  // 0x00000000
        uint32_t v12;  // 0x00000000
        uint32_t v13;  // 0x00000000
        uint32_t v14;  // 0x00000000
        uint32_t v15;  // 0x00000000
        uint32_t v16;  // 0x00000000
        uint32_t v17;  // 0x00000000
        uint32_t v18;  // 0x00000000
        uint32_t v19;  // 0x00000000
        uint32_t v20;  // 0x00000000
        uint32_t v21;  // 0x00000000
        uint32_t v22;  // 0x00000000
        uint32_t v23;  // 0x00000000
        uint32_t v24;  // 0x00000000
        uint32_t v25;  // 0x00000000
        uint32_t v26;  // 0x00000000
        uint32_t v27;  // 0x00000000
        uint32_t v28;  // 0x00000000
        uint32_t v29;  // 0x00000000
        uint32_t v30;  // 0x00000000
        uint32_t v31;  // 0x00000000
    };

    struct Navmesh
    {
        NavMeshFlags ContentFlags;
        uint32_t VersionUnk1;  // 0x00010011
        uint32_t Unused_018h;  // 0x00000000
        uint32_t Unused_01Ch;  // 0x00000000
        glm::mat4 Transform;   //(1,0,0,NaN),(0,1,0,NaN),(0,0,1,NaN),(0,0,0,NaN)
        glm::vec3 AABBSize;
        float AABBUnk;  // 0x7F800001 //NaN
        uint64_t VerticesPointer;
        uint32_t Unused_078h;  // 0x00000000
        uint32_t Unused_07Ch;  // 0x00000000
        uint64_t IndicesPointer;
        uint64_t EdgesPointer;
        uint32_t EdgesIndicesCount;
        NavMeshUintArray AdjAreaIDs;
        uint64_t PolysPointer;
        uint64_t SectorTreePointer;
        uint64_t PortalsPointer;
        uint64_t PortalLinksPointer;
        uint32_t VerticesCount;
        uint32_t PolysCount;
        uint32_t AreaID;  // X + Y*100
        uint32_t TotalBytes;
        uint32_t PointsCount;
        uint32_t PortalsCount;
        uint32_t PortalLinksCount;
        uint32_t Unused_154h;  // 0x00000000
        uint32_t Unused_158h;  // 0x00000000
        uint32_t Unused_15Ch;  // 0x00000000
        uint32_t VersionUnk2;  //2244687201 (0x85CB3561) for grid ynv's
        uint32_t Unused_164h;  // 0x00000000
        uint32_t Unused_168h;  // 0x00000000
        uint32_t Unused_16Ch;  // 0x00000000
    };

    struct NavMeshVertex
    {
        uint16_t X;
        uint16_t Y;
        uint16_t Z;
    };

    struct NavMeshList : datBase
    {
        uint32_t ItemCount;
        uint32_t Unknown_0Ch;  // 0x00000000
        uint64_t ListPartsPointer;
        uint64_t ListOffsetsPointer;
        uint32_t ListPartsCount;
        uint32_t Unknown_24h;  // 0x00000000
        uint32_t Unknown_28h;  // 0x00000000
        uint32_t Unknown_2Ch;  // 0x00000000
    };

    struct NavMeshListPart
    {
        uint64_t Pointer;
        uint32_t Count;
        uint32_t Unknown_0Ch;  // 0x00000000
    };

    struct NavMeshAABB
    {
        short MinX;
        short MaxX;
        short MinY;
        short MaxY;
        short MinZ;
        short MaxZ;
    };

    struct NavMeshSector
    {
        glm::vec4 AABBMin;  //W==NaN
        glm::vec4 AABBMax;  //W==NaN
        NavMeshAABB CellAABB;
        uint64_t DataPointer;
        uint64_t SubTree1Pointer;
        uint64_t SubTree2Pointer;
        uint64_t SubTree3Pointer;
        uint64_t SubTree4Pointer;
        uint32_t Unused_54h;  // 0x00000000
        uint32_t Unused_58h;  // 0x00000000
        uint32_t Unused_5Ch;  // 0x00000000
    };

    struct FlagsUint
    {
        uint32_t Value;
    };

    struct NavMeshPoly
    {
        uint16_t Unknown_00h;
        uint16_t IndexFlags;
        uint16_t IndexID;
        uint16_t AreaID;
        uint32_t Unused_08h;  // 0x00000000
        uint32_t Unused_0Ch;  // 0x00000000
        uint32_t Unused_10h;  // 0x00000000
        uint32_t Unused_14h;  // 0x00000000
        NavMeshAABB CellAABB;
        FlagsUint Unknown_24h;
        FlagsUint Unknown_28h;
        uint16_t PartFlags;
        uint16_t PortalLinkID;
    };

public:
    std::vector<NavMeshPoly> polys;

    bool B02_IsFootpath(NavMeshPoly* poly)
    {
        return (poly->Unknown_00h & 4) > 0;
    }

    bool B22_FootpathUnk1(NavMeshPoly* poly)
    {
        return (poly->Unknown_24h.Value & 16384) > 0;
    }
    YnvLoader(memstream& file);
    ~YnvLoader();
};
