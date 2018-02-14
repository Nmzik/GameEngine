#pragma once
#include "fstream"
#include <iostream>
#include <vector>
#include "zlib.h"
#include "glm.hpp"
#include "RpfEntry.h"

class YmapLoader
{
	struct Array_StructurePointer //16 bytes - pointer for a structure pointer array
	{
		uint32_t Pointer;
		uint32_t Unk0;
		uint16_t Count1;
		uint16_t Count2;
		uint32_t Unk1;
	};

	enum Unk_1264241711 //entity lodLevel
		: int //Key:1856311430
	{
		LODTYPES_DEPTH_HD = 0,
		LODTYPES_DEPTH_LOD = 1,
		LODTYPES_DEPTH_SLOD1 = 2,
		LODTYPES_DEPTH_SLOD2 = 3,
		LODTYPES_DEPTH_SLOD3 = 4, //thanks Tadden :D
		LODTYPES_DEPTH_ORPHANHD = 5,
		LODTYPES_DEPTH_SLOD4 = 6,
	};

	enum Unk_648413703 //entity priorityLevel
		: int //Key:2200357711
	{
		PRI_REQUIRED = 0,  //1943361227
		PRI_OPTIONAL_HIGH = 1, //3993616791
		PRI_OPTIONAL_MEDIUM = 2, //515598709
		PRI_OPTIONAL_LOW = 3, //329627604
	};

	struct CEntityDef //128 bytes, Key:1825799514
	{
		uint32_t Unused0;//0
		uint32_t Unused1;//4
		uint32_t archetypeName; //8   8: Hash: 0: archetypeName
		uint32_t flags; //12   12: UnsignedInt: 0: flags
		uint32_t guid; //16   16: UnsignedInt: 0: guid
		uint32_t Unused2;//20
		uint32_t Unused3;//24
		uint32_t Unused4;//28
		glm::vec3 position; //32   32: Float_XYZ: 0: position
		float Unused5;//44
		glm::vec4 rotation; //48   48: Float_XYZW: 0: rotation
		float scaleXY; //64   64: Float: 0: 2627937847
		float scaleZ; //68   68: Float: 0: 284916802
		int32_t parentIndex; //72   72: SignedInt: 0: parentIndex
		float lodDist; //76   76: Float: 0: lodDist
		float childLodDist; //80   80: Float: 0: childLodDist//3398912973
		Unk_1264241711 lodLevel; //84   84: IntEnum: 1264241711: lodLevel  //LODTYPES_DEPTH_
		uint32_t numChildren; //88   88: UnsignedInt: 0: numChildren//2793909385
		Unk_648413703 priorityLevel; //92   92: IntEnum: 648413703: priorityLevel//647098393
		Array_StructurePointer extensions; //96   96: Array: 0: extensions  {0: StructurePointer: 0: 256}
		int32_t ambientOcclusionMultiplier; //112   112: SignedInt: 0: ambientOcclusionMultiplier//415356295
		int32_t artificialAmbientOcclusion; //116   116: SignedInt: 0: artificialAmbientOcclusion//599844163
		uint32_t tintValue; //120   120: UnsignedInt: 0: tintValue//1015358759
		uint32_t Unused6;//124
	};
public:
	uint32_t flags;
	uint32_t Hash;
	std::vector<CEntityDef> CEntityDefs;

	YmapLoader(memstream& file, uint32_t hash);
	~YmapLoader();
};

