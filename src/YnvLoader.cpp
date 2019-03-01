#include "YnvLoader.h"

YnvLoader::YnvLoader(memstream& file)
{
    /*dtNavMeshCreateParams params;

	memset(&params, 0, sizeof(params));

	int navDataSize;
	unsigned char* navData;
	bool result = dtCreateNavMeshData(&params, &navData, &navDataSize);
	if (result)
	{
	 printf("DONE");
	}*/

    /*enum NavMeshFlags
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
	 uint32_t v06; // 0x00000000
	 uint32_t v07; // 0x00000000
	 uint32_t v08; // 0x00000000
	 uint32_t v09; // 0x00000000
	 uint32_t v10; // 0x00000000
	 uint32_t v11; // 0x00000000
	 uint32_t v12; // 0x00000000
	 uint32_t v13; // 0x00000000
	 uint32_t v14; // 0x00000000
	 uint32_t v15; // 0x00000000
	 uint32_t v16; // 0x00000000
	 uint32_t v17; // 0x00000000
	 uint32_t v18; // 0x00000000
	 uint32_t v19; // 0x00000000
	 uint32_t v20; // 0x00000000
	 uint32_t v21; // 0x00000000
	 uint32_t v22; // 0x00000000
	 uint32_t v23; // 0x00000000
	 uint32_t v24; // 0x00000000
	 uint32_t v25; // 0x00000000
	 uint32_t v26; // 0x00000000
	 uint32_t v27; // 0x00000000
	 uint32_t v28; // 0x00000000
	 uint32_t v29; // 0x00000000
	 uint32_t v30; // 0x00000000
	 uint32_t v31; // 0x00000000
	};

	struct Navmesh {
	 NavMeshFlags ContentFlags;
	 uint32_t VersionUnk1; // 0x00010011
	 uint32_t Unused_018h; // 0x00000000
	 uint32_t Unused_01Ch; // 0x00000000
	 glm::mat4 Transform; //(1,0,0,NaN),(0,1,0,NaN),(0,0,1,NaN),(0,0,0,NaN)
	 glm::vec3 AABBSize;
	 float AABBUnk; // 0x7F800001 //NaN
	 uint64_t VerticesPointer;
	 uint32_t Unused_078h; // 0x00000000
	 uint32_t Unused_07Ch; // 0x00000000
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
	 uint32_t AreaID; // X + Y*100
	 uint32_t TotalBytes;
	 uint32_t PointsCount;
	 uint32_t PortalsCount;
	 uint32_t PortalLinksCount;
	 uint32_t Unused_154h; // 0x00000000
	 uint32_t Unused_158h; // 0x00000000
	 uint32_t Unused_15Ch; // 0x00000000
	 uint32_t VersionUnk2;                //2244687201 (0x85CB3561) for grid ynv's
	 uint32_t Unused_164h; // 0x00000000
	 uint32_t Unused_168h; // 0x00000000
	 uint32_t Unused_16Ch; // 0x00000000
	};

	struct NavMeshVertex
	{
	 uint16_t X;
	 uint16_t Y;
	 uint16_t Z;
	};

	struct NavMeshList : datBase {
	 uint32_t ItemCount;
	 uint32_t Unknown_0Ch; // 0x00000000
	 uint64_t ListPartsPointer;
	 uint64_t ListOffsetsPointer;
	 uint32_t ListPartsCount;
	 uint32_t Unknown_24h; // 0x00000000
	 uint32_t Unknown_28h; // 0x00000000
	 uint32_t Unknown_2Ch; // 0x00000000
	};

	struct NavMeshListPart {
	 uint64_t Pointer;
	 uint32_t Count;
	 uint32_t Unknown_0Ch; // 0x00000000
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
	 glm::vec4 AABBMin; //W==NaN
	 glm::vec4 AABBMax; //W==NaN
	 NavMeshAABB CellAABB;
	 uint64_t DataPointer;
	 uint64_t SubTree1Pointer;
	 uint64_t SubTree2Pointer;
	 uint64_t SubTree3Pointer;
	 uint64_t SubTree4Pointer;
	 uint32_t Unused_54h; // 0x00000000
	 uint32_t Unused_58h; // 0x00000000
	 uint32_t Unused_5Ch; // 0x00000000
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
	 uint32_t Unused_08h; // 0x00000000
	 uint32_t Unused_0Ch; // 0x00000000
	 uint32_t Unused_10h; // 0x00000000
	 uint32_t Unused_14h; // 0x00000000
	 NavMeshAABB CellAABB;
	 FlagsUint Unknown_24h;
	 FlagsUint Unknown_28h;
	 uint16_t PartFlags;
	 uint16_t PortalLinkID;
	};

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

	NavMeshSector SectorTree;

	SYSTEM_BASE_PTR(navmesh.SectorTreePointer);
	file.seekg(navmesh.SectorTreePointer);
	file.read((char*)&SectorTree, sizeof(NavMeshSector));

	NavMeshPoly poly;
	SYSTEM_BASE_PTR(navmesh.PolysPointer);
	//file.seekg(navmesh.SectorTreePointer);
	//file.read((char*)&SectorTree, sizeof(NavMeshSector));
	//NEED TO LOAD POLYS TO MAKE IT WORK!


	rcHeightfield *rcheightfield;

	rcCompactHeightfield *rccompactheightfield;

	rcContourSet *rccontourset;

	rcPolyMesh *rcpolymesh;

	rcPolyMeshDetail *rcpolymeshdetail;

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
	 rcconfig.ch)) {
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


	rcRasterizeTriangles(&rc,
	 (float*)&VerticesVector[0],
	 VerticesVector.size(),
	 (int*)&IndicesVector[0],
	 triangle_flags,
	 ntris,
	 *rcheightfield,
	 rcconfig.walkableClimb);


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

	rcBuildCompactHeightfield(&rc, rcconfig.walkableHeight,
	 rcconfig.walkableClimb,
	 *rcheightfield,
	 *rccompactheightfield);

	rcFreeHeightField(rcheightfield);
	rcheightfield = NULL;

	// Erode the walkable area by agent radius.
	rcErodeWalkableArea(&rc,
	 rcconfig.walkableRadius,
	 *rccompactheightfield);


	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	rcBuildDistanceField(&rc, *rccompactheightfield);


	// Partition the walkable surface into simple regions without holes.
	rcBuildRegions(&rc, *rccompactheightfield,
	 0,
	 rcconfig.minRegionArea,
	 rcconfig.mergeRegionArea);


	// Step 5. Trace and simplify region contours.
	rccontourset = rcAllocContourSet();

	rcBuildContours(&rc, *rccompactheightfield,
	 rcconfig.maxSimplificationError,
	 rcconfig.maxEdgeLen,
	 *rccontourset);


	// Step 6. Build polygons mesh from contours.
	rcpolymesh = rcAllocPolyMesh();

	if (!rcBuildPolyMesh(&rc, *rccontourset,
	 rcconfig.maxVertsPerPoly,
	 *rcpolymesh)) {
	 printf("ERROR");
	}


	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	rcpolymeshdetail = rcAllocPolyMeshDetail();
	if (!rcBuildPolyMeshDetail(&rc, *rcpolymesh,
	 *rccompactheightfield,
	 rcconfig.detailSampleDist,
	 rcconfig.detailSampleMaxError,
	 *rcpolymeshdetail)) {
	 printf("ERROR");
	}


	rcFreeCompactHeightfield(rccompactheightfield);
	rccompactheightfield = NULL;

	rcFreeContourSet(rccontourset);
	rccontourset = NULL;

	int i = 0;
	if (rcconfig.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
	 dtNavMeshCreateParams dtnavmeshcreateparams;

	 unsigned char *nav_data = NULL;

	 int nav_data_size = 0;

	 i = 0;
	 while (i != rcpolymesh->npolys)
	 {
	  if (rcpolymesh->areas[i] == RC_WALKABLE_AREA)
	  {
	   rcpolymesh->areas[i] = 0;
	   rcpolymesh->flags[i] = 0x01;
	  }

	  ++i;
	 }


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


	 dtCreateNavMeshData(&dtnavmeshcreateparams,
	  &nav_data,
	  &nav_data_size);

	 if (!nav_data) printf("ERROR???");;

	 dtNavMesh *dtnavmesh = dtAllocNavMesh();

	 dtnavmesh->init(nav_data,
	  nav_data_size,
	  DT_TILE_FREE_DATA);

	 rcFreePolyMesh(rcpolymesh);
	 rcpolymesh = NULL;

	 rcFreePolyMeshDetail(rcpolymeshdetail);
	 rcpolymeshdetail = NULL;
	}
	*/
}

YnvLoader::~YnvLoader()
{
}
