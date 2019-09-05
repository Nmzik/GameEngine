#include "YnvLoader.h"

YnvLoader::YnvLoader(memstream& file)
{
    std::vector<NavMeshVertex> navMeshVertices;
    std::vector<uint16_t> navMeshIndices;

    ResourceFileBase resourceFileBase;
    file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

    Navmesh navmesh;
    file.read((char*)&navmesh, sizeof(Navmesh));

    navMeshVertices.reserve(navmesh.VerticesCount);
    navMeshIndices.reserve(navmesh.EdgesIndicesCount);

    SYSTEM_BASE_PTR(navmesh.VerticesPointer);

    SYSTEM_BASE_PTR(navmesh.IndicesPointer);

    ////
    file.seekg(navmesh.VerticesPointer);

    NavMeshList navMeshList;
    file.read((char*)&navMeshList, sizeof(NavMeshList));

    SYSTEM_BASE_PTR(navMeshList.ListPartsPointer);

    file.seekg(navMeshList.ListPartsPointer);

    for (uint32_t i = 0; i < navMeshList.ListPartsCount; i++)
    {
        NavMeshListPart navMeshListPart;
        file.read((char*)&navMeshListPart, sizeof(NavMeshListPart));

        uint64_t navMeshListPartPointer = file.tellg();

        SYSTEM_BASE_PTR(navMeshListPart.Pointer);

        file.seekg(navMeshListPart.Pointer);

        for (uint32_t i = 0; i < navMeshListPart.Count; i++)
        {
            NavMeshVertex navMeshVertex;
            file.read((char*)&navMeshVertex, sizeof(navMeshVertex));

            navMeshVertices.push_back(navMeshVertex);
        }

        file.seekg(navMeshListPartPointer);
    }

    ///
    file.seekg(navmesh.IndicesPointer);

    //NavMeshList navMeshList;
    file.read((char*)&navMeshList, sizeof(NavMeshList));

    SYSTEM_BASE_PTR(navMeshList.ListPartsPointer);

    file.seekg(navMeshList.ListPartsPointer);

    for (uint32_t i = 0; i < navMeshList.ListPartsCount; i++)
    {
        NavMeshListPart navMeshListPart;
        file.read((char*)&navMeshListPart, sizeof(NavMeshListPart));

        uint64_t navMeshListPartPointer = file.tellg();

        SYSTEM_BASE_PTR(navMeshListPart.Pointer);

        file.seekg(navMeshListPart.Pointer);

        for (uint32_t i = 0; i < navMeshListPart.Count; i++)
        {
            uint16_t Index;
            file.read((char*)&Index, sizeof(uint16_t));

            navMeshIndices.push_back(Index);
        }

        file.seekg(navMeshListPartPointer);
    }

    SYSTEM_BASE_PTR(navmesh.PolysPointer);

    file.seekg(navmesh.PolysPointer);
    //NavMeshList navMeshList;
    file.read((char*)&navMeshList, sizeof(NavMeshList));

    SYSTEM_BASE_PTR(navMeshList.ListPartsPointer);
    file.seekg(navMeshList.ListPartsPointer);

    for (uint32_t i = 0; i < navMeshList.ListPartsCount; i++)
    {
        NavMeshListPart navMeshListPart;
        file.read((char*)&navMeshListPart, sizeof(NavMeshListPart));

        uint64_t navMeshListPartPointer = file.tellg();

        SYSTEM_BASE_PTR(navMeshListPart.Pointer);
        file.seekg(navMeshListPart.Pointer);

        for (uint32_t i = 0; i < navMeshListPart.Count; i++)
        {
            NavMeshPoly poly;
            file.read((char*)&poly, sizeof(NavMeshPoly));

            polys.push_back(poly);
        }

        file.seekg(navMeshListPartPointer);
    }

    SYSTEM_BASE_PTR(navmesh.SectorTreePointer);
    file.seekg(navmesh.SectorTreePointer);

    NavMeshSector SectorTree;
    file.read((char*)&SectorTree, sizeof(NavMeshSector));
    //NEED TO LOAD POLYS TO MAKE IT WORK!

    rcHeightfield* rcheightfield;

    rcCompactHeightfield* rccompactheightfield;

    rcContourSet* rccontourset;

    rcPolyMesh* rcpolymesh;

    rcPolyMeshDetail* rcpolymeshdetail;

    struct NAVIGATIONCONFIGURATION
    {
        float cell_size;

        float cell_height;

        float agent_height;

        float agent_radius;

        float agent_max_climb;

        float agent_max_slope;

        float region_min_size;

        float region_merge_size;

        float edge_max_len;

        float edge_max_error;

        float vert_per_poly;

        float detail_sample_dst;

        float detail_sample_max_error;
    };

    std::vector<glm::vec3> VerticesVector;
    std::vector<int32_t> IndicesVector;

    const float ushortMaxValue = 65535;
    //CONVERT USHORT TO float
    for (int i = 0; i < navMeshVertices.size(); i++)
    {
        glm::vec3 posoffset(SectorTree.AABBMin.x, SectorTree.AABBMin.y, SectorTree.AABBMin.z);
        glm::vec3 Vector(navMeshVertices[i].X / ushortMaxValue, navMeshVertices[i].Y / ushortMaxValue, navMeshVertices[i].Z / ushortMaxValue);
        glm::vec3 result(posoffset + Vector * navmesh.AABBSize);
        VerticesVector.push_back(result);
    }

    //convert indices
    for (int i = 0; i < navMeshIndices.size(); i++)
    {
        IndicesVector.push_back(navMeshIndices[i]);
    }

    NAVIGATIONCONFIGURATION navigationconfiguration;

    navigationconfiguration.cell_size = 0.3f;
    navigationconfiguration.cell_height = 0.2f;
    navigationconfiguration.agent_height = 2.0f;
    navigationconfiguration.agent_radius = 0.4f;
    navigationconfiguration.agent_max_climb = 0.9f;
    navigationconfiguration.agent_max_slope = 45.0f;
    navigationconfiguration.region_min_size = 50.0f;
    navigationconfiguration.region_merge_size = 20.0f;
    navigationconfiguration.edge_max_len = 12.0f;
    navigationconfiguration.edge_max_error = 1.3f;
    navigationconfiguration.vert_per_poly = 6.0f;
    navigationconfiguration.detail_sample_dst = 6.0f;
    navigationconfiguration.detail_sample_max_error = 1.0f;

    // Step 1. Initialize build config.
    rcConfig rcconfig;
    memset(&rcconfig, 0, sizeof(rcConfig));
    rcconfig.cs = navigationconfiguration.cell_size;
    rcconfig.ch = navigationconfiguration.cell_height;
    rcconfig.walkableHeight = (int)ceilf(navigationconfiguration.agent_height / rcconfig.ch);
    rcconfig.walkableRadius = (int)ceilf(navigationconfiguration.agent_radius / rcconfig.cs);
    rcconfig.walkableClimb = (int)floorf(navigationconfiguration.agent_max_climb / rcconfig.ch);
    rcconfig.walkableSlopeAngle = navigationconfiguration.agent_max_slope;
    rcconfig.minRegionArea = (int)rcSqr(navigationconfiguration.region_min_size);
    rcconfig.mergeRegionArea = (int)rcSqr(navigationconfiguration.region_merge_size);
    rcconfig.maxEdgeLen = (int)(navigationconfiguration.edge_max_len / rcconfig.cs);
    rcconfig.maxSimplificationError = navigationconfiguration.edge_max_error;
    rcconfig.maxVertsPerPoly = (int)navigationconfiguration.vert_per_poly;
    rcconfig.detailSampleDist = rcconfig.cs * navigationconfiguration.detail_sample_dst;
    rcconfig.detailSampleMaxError = rcconfig.ch * navigationconfiguration.detail_sample_max_error;
    //
    rcContext rc;
    rcCalcBounds((float*)&VerticesVector[0],
                 VerticesVector.size(),
                 rcconfig.bmin,
                 rcconfig.bmax);

    rcCalcGridSize(rcconfig.bmin,
                   rcconfig.bmax,
                   rcconfig.cs,
                   &rcconfig.width,
                   &rcconfig.height);

    // Step 2. Rasterize input polygon soup.
    // Allocate voxel heightfield where we rasterize our input data to.
    rcheightfield = rcAllocHeightfield();

    if (!rcCreateHeightfield(&rc, *rcheightfield,
                             rcconfig.width,
                             rcconfig.height,
                             rcconfig.bmin,
                             rcconfig.bmax,
                             rcconfig.cs,
                             rcconfig.ch))
    {
        printf("ERROR");
    }

    // Allocate array that can hold triangle area types.
    // If you have multiple meshes you need to process, allocate
    // and array which can hold the max number of triangles you need to process.

    int ntris = IndicesVector.size() / 3;

    uint8_t* triangle_flags = new uint8_t[ntris];
    memset(triangle_flags, 0, ntris);
    // Find triangles which are walkable based on their slope and rasterize them.
    // If your input data is multiple meshes, you can transform them here, calculate
    // the are type for each of the meshes and rasterize them.
    rcMarkWalkableTriangles(&rc,
                            rcconfig.walkableSlopeAngle,
                            (float*)&VerticesVector[0],
                            VerticesVector.size(),
                            (int*)&IndicesVector[0],
                            ntris,
                            triangle_flags);

    if (!rcRasterizeTriangles(&rc,
                              (float*)&VerticesVector[0],
                              VerticesVector.size(),
                              (int*)&IndicesVector[0],
                              triangle_flags,
                              ntris,
                              *rcheightfield,
                              rcconfig.walkableClimb))
        printf("ERROR");

    delete[] triangle_flags;
    triangle_flags = NULL;

    // Step 3. Filter walkables surfaces.
    // Once all geoemtry is rasterized, we do initial pass of filtering to
    // remove unwanted overhangs caused by the conservative rasterization
    // as well as filter spans where the character cannot possibly stand.
    rcFilterLowHangingWalkableObstacles(&rc,
                                        rcconfig.walkableClimb,
                                        *rcheightfield);
    rcFilterLedgeSpans(&rc,
                       rcconfig.walkableHeight,
                       rcconfig.walkableClimb,
                       *rcheightfield);

    rcFilterWalkableLowHeightSpans(&rc,
                                   rcconfig.walkableHeight,
                                   *rcheightfield);

    // Step 4. Partition walkable surface to simple regions.
    // Compact the heightfield so that it is faster to handle from now on.
    // This will result more cache coherent data as well as the neighbours
    // between walkable cells will be calculated.
    rccompactheightfield = rcAllocCompactHeightfield();

    if (!rcBuildCompactHeightfield(&rc, rcconfig.walkableHeight,
                                   rcconfig.walkableClimb,
                                   *rcheightfield,
                                   *rccompactheightfield))
        printf("ERROR");

    rcFreeHeightField(rcheightfield);
    rcheightfield = NULL;

    // Erode the walkable area by agent radius.
    if (!rcErodeWalkableArea(&rc,
                             rcconfig.walkableRadius,
                             *rccompactheightfield))
        printf("ERROR");

    // Prepare for region partitioning, by calculating distance field along the walkable surface.
    if (!rcBuildDistanceField(&rc, *rccompactheightfield))
        printf("ERROR");

    // Partition the walkable surface into simple regions without holes.
    if (!rcBuildRegions(&rc, *rccompactheightfield,
                        0,
                        rcconfig.minRegionArea,
                        rcconfig.mergeRegionArea))
        printf("ERROR");

    // Step 5. Trace and simplify region contours.
    rccontourset = rcAllocContourSet();

    if (!rcBuildContours(&rc, *rccompactheightfield,
                         rcconfig.maxSimplificationError,
                         rcconfig.maxEdgeLen,
                         *rccontourset))
        printf("ERROR");

    // Step 6. Build polygons mesh from contours.
    rcpolymesh = rcAllocPolyMesh();

    if (!rcBuildPolyMesh(&rc, *rccontourset,
                         rcconfig.maxVertsPerPoly,
                         *rcpolymesh))
    {
        printf("ERROR");
    }

    // Step 7. Create detail mesh which allows to access approximate height on each polygon.
    rcpolymeshdetail = rcAllocPolyMeshDetail();
    if (!rcBuildPolyMeshDetail(&rc, *rcpolymesh,
                               *rccompactheightfield,
                               rcconfig.detailSampleDist,
                               rcconfig.detailSampleMaxError,
                               *rcpolymeshdetail))
    {
        printf("ERROR");
    }

    rcFreeCompactHeightfield(rccompactheightfield);
    rccompactheightfield = NULL;

    rcFreeContourSet(rccontourset);
    rccontourset = NULL;

    int i = 0;
    if (rcconfig.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
    {
        unsigned char* nav_data = NULL;
        int nav_data_size = 0;

        enum SamplePolyAreas
        {
            SAMPLE_POLYAREA_GROUND,
            SAMPLE_POLYAREA_WATER,
            SAMPLE_POLYAREA_ROAD,
            SAMPLE_POLYAREA_DOOR,
            SAMPLE_POLYAREA_GRASS,
        };

        enum SamplePolyFlags
        {
            SAMPLE_POLYFLAGS_WALK = 0x01,      // Ability to walk (ground, grass, road)
            SAMPLE_POLYFLAGS_SWIM = 0x02,      // Ability to swim (water).
            SAMPLE_POLYFLAGS_DOOR = 0x04,      // Ability to move through doors.
            SAMPLE_POLYFLAGS_JUMP = 0x08,      // Ability to jump.
            SAMPLE_POLYFLAGS_DISABLED = 0x10,  // Disabled polygon
            SAMPLE_POLYFLAGS_ALL = 0xffff      // All abilities.
        };

        // Update poly flags from areas.
        for (int i = 0; i < rcpolymesh->npolys; ++i)
        {
            if (rcpolymesh->areas[i] == RC_WALKABLE_AREA)
                rcpolymesh->areas[i] = SAMPLE_POLYAREA_GROUND;

            if (rcpolymesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
                rcpolymesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
                rcpolymesh->areas[i] == SAMPLE_POLYAREA_ROAD)
            {
                rcpolymesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
            }
            else if (rcpolymesh->areas[i] == SAMPLE_POLYAREA_WATER)
            {
                rcpolymesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
            }
            else if (rcpolymesh->areas[i] == SAMPLE_POLYAREA_DOOR)
            {
                rcpolymesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
            }
        }

        dtNavMeshCreateParams dtnavmeshcreateparams;
        memset(&dtnavmeshcreateparams, 0, sizeof(dtNavMeshCreateParams));

        dtnavmeshcreateparams.verts = rcpolymesh->verts;
        dtnavmeshcreateparams.vertCount = rcpolymesh->nverts;
        dtnavmeshcreateparams.polys = rcpolymesh->polys;
        dtnavmeshcreateparams.polyAreas = rcpolymesh->areas;
        dtnavmeshcreateparams.polyFlags = rcpolymesh->flags;
        dtnavmeshcreateparams.polyCount = rcpolymesh->npolys;
        dtnavmeshcreateparams.nvp = rcpolymesh->nvp;

        dtnavmeshcreateparams.detailMeshes = rcpolymeshdetail->meshes;
        dtnavmeshcreateparams.detailVerts = rcpolymeshdetail->verts;
        dtnavmeshcreateparams.detailVertsCount = rcpolymeshdetail->nverts;
        dtnavmeshcreateparams.detailTris = rcpolymeshdetail->tris;
        dtnavmeshcreateparams.detailTriCount = rcpolymeshdetail->ntris;

        dtnavmeshcreateparams.walkableHeight = navigationconfiguration.agent_height;
        dtnavmeshcreateparams.walkableRadius = navigationconfiguration.agent_radius;
        dtnavmeshcreateparams.walkableClimb = navigationconfiguration.agent_max_climb;

        rcVcopy(dtnavmeshcreateparams.bmin, rcpolymesh->bmin);
        rcVcopy(dtnavmeshcreateparams.bmax, rcpolymesh->bmax);

        dtnavmeshcreateparams.cs = rcconfig.cs;
        dtnavmeshcreateparams.ch = rcconfig.ch;

        if (!dtCreateNavMeshData(&dtnavmeshcreateparams,
                                 &nav_data,
                                 &nav_data_size))
            printf("ERROR");

        dtNavMesh* dtnavmesh = dtAllocNavMesh();

        dtnavmesh->init(nav_data,
                        nav_data_size,
                        DT_TILE_FREE_DATA);

        rcFreePolyMesh(rcpolymesh);
        rcpolymesh = NULL;

        rcFreePolyMeshDetail(rcpolymeshdetail);
        rcpolymeshdetail = NULL;
    }
}

YnvLoader::~YnvLoader()
{
}
