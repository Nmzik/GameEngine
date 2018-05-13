#pragma once
#include "YdrLoader.h"
#include "Meta.h"
#include "Object.h"

struct CMloInstanceDef //160 bytes, Key:2151576752
{
	CEntityDef CEntityDef;
	uint32_t groupId; //128   128: UnsignedInt: 0: 2501631252
	uint32_t floorId; //132   132: UnsignedInt: 0: floorId//2187650609
	Array_uint defaultEntitySets; //136   136: Array: 0: defaultEntitySets//1407157833  {0: Hash: 0: 256}
	uint32_t numExitPortals; //152   152: UnsignedInt: 0: numExitPortals//528711607
	uint32_t MLOInstflags; //156   156: UnsignedInt: 0: MLOInstflags//3761966250
};

struct CCarGen //80 bytes, Key:2345238261
{
	uint32_t Unused0;//0
	uint32_t Unused1;//4
	uint32_t Unused2;//8
	uint32_t Unused3;//12
	glm::vec3 position; //16   16: Float_XYZ: 0: position
	float Unused4;//28
	float orientX; //32   32: Float: 0: orientX=735213009
	float orientY; //36   36: Float: 0: orientY=979440342
	float perpendicularLength; //40   40: Float: 0: perpendicularLength=124715667
	uint32_t  carModel; //44   44: Hash: 0: carModel
	uint32_t flags; //48   48: UnsignedInt: 0: flags   ///  _CP_: looks like flag 1879051873 in cargens forces to spawn a vehicle
	int32_t  bodyColorRemap1; //52   52: SignedInt: 0: bodyColorRemap1=1429703670
	int32_t  bodyColorRemap2; //56   56: SignedInt: 0: bodyColorRemap2=1254848286
	int32_t  bodyColorRemap3; //60   60: SignedInt: 0: bodyColorRemap3=1880965569
	int32_t  bodyColorRemap4; //64   64: SignedInt: 0: bodyColorRemap4=1719152247
	int32_t  popGroup; //68   68: Hash: 0: popGroup=911358791
	int8_t livery; //72   72: SignedByte: 0: livery
	uint8_t Unused5;//73
	int16_t Unused6;//74
	uint32_t Unused7;//76
};

///YMAP REAL DATA

struct rage__fwInstancedMapData //48 bytes, Key:1836780118
{
	uint32_t Unused0;//0
	uint32_t Unused1;//4
	uint32_t ImapLink; //8   8: Hash: 0: ImapLink//2142127586
	uint32_t Unused2;//12
	Array_Structure PropInstanceList; //16   16: Array: 0: PropInstanceList//3551474528  {0: Structure: rage__fwPropInstanceListDef: 256}
	Array_Structure GrassInstanceList;
};
struct CLODLight //136 bytes, Key:2325189228
{
	uint32_t Unused0;//0
	uint32_t Unused1;//4
	Array_Structure direction; //8   8: Array: 0: direction  {0: Structure: VECTOR3: 256}
	Array_float falloff; //24   24: Array: 0: falloff  {0: Float: 0: 256}
	Array_float falloffExponent; //40   40: Array: 0: falloffExponent  {0: Float: 0: 256}
	Array_uint timeAndStateFlags; //56   56: Array: 0: timeAndStateFlags=3112418278  {0: UnsignedInt: 0: 256}
	Array_uint hash; //72   72: Array: 0: hash  {0: UnsignedInt: 0: 256}
	Array_byte coneInnerAngle; //88   88: Array: 0: coneInnerAngle//1163671864  {0: UnsignedByte: 0: 256}
	Array_byte coneOuterAngleOrCapExt; //104   104: Array: 0: coneOuterAngleOrCapExt=3161894080  {0: UnsignedByte: 0: 256}
	Array_byte coronaIntensity; //120   120: Array: 0: coronaIntensity//2292363771  {0: UnsignedByte: 0: 256}
};
struct CDistantLODLight //48 bytes, Key:2820908419
{
	uint32_t Unused0;//0
	uint32_t Unused1;//4
	Array_Structure position; //8   8: Array: 0: position  {0: Structure: VECTOR3: 256}
	Array_uint RGBI; //24   24: Array: 0: RGBI  {0: UnsignedInt: 0: 256}
	uint16_t numStreetLights; //40   40: UnsignedShort: 0: numStreetLights//3708891211
	uint16_t category; //42   42: UnsignedShort: 0: category//2052871693
	uint32_t Unused2;//44
};

struct CBlockDesc //72 bytes, Key:2015795449
{
	uint32_t version; //0   0: UnsignedInt: 0: version
	uint32_t flags; //4   4: UnsignedInt: 0: flags
	CharPointer name; //8   8: CharPointer: 0: name
	CharPointer exportedBy; //24   24: CharPointer: 0: exportedBy//1983184981
	CharPointer owner; //40   40: CharPointer: 0: owner
	CharPointer time; //56   56: CharPointer: 0: time
};

struct CMapData //512 bytes, Key:3448101671
{
	uint32_t Unused0;//0
	uint32_t Unused1;//4
	uint32_t name; //8   8: Hash: 0: name
	uint32_t parent; //12   12: Hash: 0: parent
	uint32_t flags; //16   16: UnsignedInt: 0: flags
	uint32_t contentFlags; //20   20: UnsignedInt: 0: contentFlags//1785155637
	uint32_t Unused2;//24
	uint32_t Unused3;//28
	glm::vec3 streamingExtentsMin; //32   32: Float_XYZ: 0: streamingExtentsMin//3710026271
	float Unused4;//44
	glm::vec3 streamingExtentsMax; //48   48: Float_XYZ: 0: streamingExtentsMax//2720965429
	float Unused5;//60
	glm::vec3 entitiesExtentsMin; //64   64: Float_XYZ: 0: entitiesExtentsMin//477478129
	float Unused6;//76
	glm::vec3 entitiesExtentsMax; //80   80: Float_XYZ: 0: entitiesExtentsMax//1829192759
	float Unused7;//92
	Array_StructurePointer entities; //96   96: Array: 0: entities  {0: StructurePointer: 0: 256}
	Array_Structure containerLods; //112   112: Array: 0: containerLods//2935983381  {0: Structure: 372253349: 256}
	Array_Structure boxOccluders; //128   128: Array: 0: boxOccluders//3983590932  {0: Structure: SectionUNKNOWN7: 256}
	Array_Structure occludeModels; //144   144: Array: 0: occludeModels//2132383965  {0: Structure: SectionUNKNOWN5: 256}
	Array_uint physicsDictionaries; //160   160: Array: 0: physicsDictionaries//949589348  {0: Hash: 0: 256}
	rage__fwInstancedMapData instancedData; //176   176: Structure: rage__fwInstancedMapData: instancedData//2569067561
	Array_Structure timeCycleModifiers; //224   224: Array: 0: timeCycleModifiers  {0: Structure: CTimeCycleModifier: 256}
	Array_Structure carGenerators; //240   240: Array: 0: carGenerators//3254823756  {0: Structure: CCarGen: 256}
	CLODLight LODLightsSOA; //256   256: Structure: CLODLight: LODLightsSOA//1774371066
	CDistantLODLight DistantLODLightsSOA; //392   392: Structure: CDistantLODLight: DistantLODLightsSOA//2954466641
	CBlockDesc block; //440   440: Structure: CBlockDesc//3072355914: block
};

class YmapLoader
{
public:
	YmapLoader * next;

	uint32_t time;
	CMapData _CMapData;
	std::vector<CMloInstanceDef> CMloInstanceDefs;
	std::vector<Object> Objects;
	std::vector<CCarGen> CCarGens;

	void Init(memstream & file);
};

class YmapPool
{
public:
	YmapPool();
	~YmapPool();

	YmapLoader* Load(memstream& file);
	void Remove(YmapLoader* ymap);

	YmapLoader ymaps[500];

private:
	YmapLoader * firstAvailable_;
};

