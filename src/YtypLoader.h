#pragma once
#include "Meta.h"

enum Unk_1991964615 //archetype assetType
	: int //Key:1866031916
{
	ASSET_TYPE_UNINITIALIZED = 0, //189734893
	ASSET_TYPE_FRAGMENT = 1, //571047911
	ASSET_TYPE_DRAWABLE = 2, //130075505
	ASSET_TYPE_DRAWABLEDICTIONARY = 3, //1580165652
	ASSET_TYPE_ASSETLESS = 4, //4161085041
};

struct CBaseArchetypeDef //144 bytes, Key:2411387556
{
	uint32_t Unused00;//0
	uint32_t Unused01;//4
	float lodDist; //8   8: Float: 0: lodDist
	uint32_t flags; //12   12: UnsignedInt: 0: flags
	uint32_t specialAttribute; //16   16: UnsignedInt: 0: specialAttribute
	uint32_t Unused02;//20
	uint32_t Unused03;//24
	uint32_t Unused04;//28
	glm::vec3 bbMin; //32   32: Float_XYZ: 0: bbMin
	float Unused05;//44
	glm::vec3 bbMax; //48   48: Float_XYZ: 0: bbMax
	float Unused06;//60
	glm::vec3 bsCentre; //64   64: Float_XYZ: 0: bsCentre
	float Unused07;//76
	float bsRadius; //80   80: Float: 0: bsRadius
	float hdTextureDist; //84   84: Float: 0: hdTextureDist//2908576588
	uint32_t name; //88   88: Hash: 0: name
	uint32_t textureDictionary; //92   92: Hash: 0: textureDictionary
	uint32_t clipDictionary; //96   96: Hash: 0: clipDictionary//424089489
	uint32_t drawableDictionary; //100   100: Hash: 0: drawableDictionary
	uint32_t physicsDictionary; //104   104: Hash: 0: physicsDictionary//3553040380
	Unk_1991964615 assetType; //108   108: IntEnum: 1991964615: assetType
	uint32_t assetName; //112   112: Hash: 0: assetName
	uint32_t Unused08;//116
	Array_StructurePointer extensions; //120   120: Array: 0: extensions  {0: StructurePointer: 0: 256}
	uint32_t Unused09;//136
	uint32_t Unused10;//140
};

struct CTimeArchetypeDefData
{
	uint32_t timeFlags; //144   144: UnsignedInt: 0: timeFlags//2248791340
	uint32_t Unused11;//148
	uint32_t Unused12;//152
	uint32_t Unused13;//156
};

struct CTimeArchetypeDef //160 bytes, Key:2520619910
{
	CBaseArchetypeDef _BaseArchetypeDef;
	CTimeArchetypeDefData _TimeArchetypeDef;
};

struct CMloArchetypeDefData
{
	uint32_t mloFlags; //144   144: UnsignedInt: 0: mloFlags//3590839912
	uint32_t Unused11;//148
	Array_StructurePointer entities; //152   152: Array: 0: entities  {0: StructurePointer: 0: 256}
	Array_Structure rooms; //168   168: Array: 0: rooms  {0: Structure: CMloRoomDef: 256}
	Array_Structure portals; //184   184: Array: 0: portals//2314725778  {0: Structure: CMloPortalDef: 256}
	Array_Structure entitySets; //200   200: Array: 0: entitySets//1169996080  {0: Structure: CMloEntitySet: 256}
	Array_Structure timeCycleModifiers; //216   216: Array: 0: timeCycleModifiers  {0: Structure: CMloTimeCycleModifier: 256}
	uint32_t Unused12;//232
	uint32_t Unused13;//236
};

struct CMloArchetypeDef {
	CBaseArchetypeDef _BaseArchetypeDef;
	CMloArchetypeDefData _MloArchetypeDefData;
};

struct CMapTypes //80 bytes, Key:2608875220
{
	uint32_t Unused0;//0
	uint32_t Unused1;//4
	Array_StructurePointer extensions; //8   8: Array: 0: extensions  {0: StructurePointer: 0: 256}
	Array_StructurePointer archetypes; //24   24: Array: 0: archetypes  {0: StructurePointer: 0: 256}
	uint32_t name; //40   40: Hash: 0: name
	uint32_t Unused2;//44
	Array_uint dependencies; //48   48: Array: 0: dependencies//1013942340  {0: Hash: 0: 256}
	Array_Structure compositeEntityTypes;
};

class Archetype {
public:
	CBaseArchetypeDef BaseArchetypeDef;

	virtual uint32_t GetType() {
		return 0;
	}
};

class TimeArchetype : public Archetype {
public:
	CTimeArchetypeDefData TimeArchetypeDef;

	virtual uint32_t GetType() {
		return 1;
	}
};

class MloArchetype : public Archetype {
public:
	CMloArchetypeDefData MloArchetypeDef;

	virtual uint32_t GetType() {
		return 2;
	}
};

class YtypLoader
{
public:
	std::vector<Archetype*> ArchetypeDefs;
	//std::vector<CEntityDef> CEntityDefs;
	YtypLoader(memstream2& file);
	~YtypLoader();
};

