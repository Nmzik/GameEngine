#include "YmapLoader.h"

#define SYSTEM_BASE 0x50000000
#define GRAPHICS_BASE 0x60000000

struct MetaStructureEntryInfo_s {
	uint32_t EntryNameHash;
	int32_t DataOffset;
	uint8_t DataType;
	uint8_t Unknown_9h;
	int16_t ReferenceTypeIndex;
	uint32_t ReferenceKey;
};

struct MetaEnumEntryInfo_s {
	uint32_t EntryNameHash;
	int32_t EntryValue;
};

class MetaStructureInfo {
public:
	struct {
		uint32_t StructureNameHash;
		uint32_t StructureKey;
		uint32_t Unknown_8h;
		uint32_t Unknown_Ch; //0x00000000;
		uint64_t EntriesPointer;
		int32_t StructureSize;
		int16_t Unknown_1Ch; //0x0000;
		int16_t EntriesCount;
	} MetaStructureInfo_struct;

	std::vector<MetaStructureEntryInfo_s> entryInfos;

	MetaStructureInfo(memstream& file) {
		file.read((char*)&MetaStructureInfo_struct, sizeof(MetaStructureInfo_struct));

		uint64_t pos = file.tellg();

		if ((MetaStructureInfo_struct.EntriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			MetaStructureInfo_struct.EntriesPointer = MetaStructureInfo_struct.EntriesPointer & ~0x50000000;
		}
		if ((MetaStructureInfo_struct.EntriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			MetaStructureInfo_struct.EntriesPointer = MetaStructureInfo_struct.EntriesPointer & ~0x60000000;
		}

		file.seekg(MetaStructureInfo_struct.EntriesPointer);

		for (int i = 0; i < MetaStructureInfo_struct.EntriesCount; i++)
		{
			MetaStructureEntryInfo_s entry;
			file.read((char*)&entry, sizeof(MetaStructureEntryInfo_s));
			entryInfos.push_back(entry);
		}
		file.seekg(pos);
	}

};

class MetaEnumInfo {
public:
	struct {
		uint32_t EnumNameHash;
		uint32_t EnumKey;
		uint64_t EntriesPointer;
		int32_t EntriesCount;
		int32_t Unknown_14h; //0x00000000;
	} MetaEnumInfo_struct;

	std::vector<MetaEnumEntryInfo_s> infos;

	MetaEnumInfo(memstream& file) {
		file.read((char*)&MetaEnumInfo_struct, sizeof(MetaEnumInfo_struct));

		if ((MetaEnumInfo_struct.EntriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			MetaEnumInfo_struct.EntriesPointer = MetaEnumInfo_struct.EntriesPointer & ~0x50000000;
		}
		if ((MetaEnumInfo_struct.EntriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			MetaEnumInfo_struct.EntriesPointer = MetaEnumInfo_struct.EntriesPointer & ~0x60000000;
		}

		file.seekg(MetaEnumInfo_struct.EntriesPointer);

		for (int i = 0; i < MetaEnumInfo_struct.EntriesCount; i++)
		{
			MetaEnumEntryInfo_s info;
			file.read((char*)&info, sizeof(MetaEnumEntryInfo_s));
			infos.push_back(info);
		}
	}
};

class MetaDataBlock{
public:
	struct {
		uint32_t StructureNameHash;
		int32_t DataLength;
		uint64_t DataPointer;
	} MetaDataBlock_struct;

	uint8_t* Data;

	MetaDataBlock(memstream& file) {
		file.read((char*)&MetaDataBlock_struct, sizeof(MetaDataBlock_struct));

		uint64_t curPos = file.tellg();

		Data = new uint8_t[MetaDataBlock_struct.DataLength];

		if ((MetaDataBlock_struct.DataPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			MetaDataBlock_struct.DataPointer = MetaDataBlock_struct.DataPointer & ~0x50000000;
		}
		if ((MetaDataBlock_struct.DataPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			MetaDataBlock_struct.DataPointer = MetaDataBlock_struct.DataPointer & ~0x60000000;
		}

		file.seekg(MetaDataBlock_struct.DataPointer);

		file.read((char*)&Data[0], MetaDataBlock_struct.DataLength);

		file.seekg(curPos);
	}
};

class Meta {
public:
	struct {
		int32_t Unknown_10h;
		uint16_t Unknown_14h;
		uint8_t HasUselessData;
		uint8_t Unknown_17h;
		int32_t Unknown_18h;
		int32_t RootBlockIndex;
		uint64_t StructureInfosPointer;
		uint64_t EnumInfosPointer;
		uint64_t DataBlocksPointer;
		uint64_t NamePointer;
		uint64_t UselessPointer;
		uint16_t StructureInfosCount;
		uint16_t EnumInfosCount;
		uint16_t DataBlocksCount;
		uint16_t Unknown_4Eh;
		uint32_t Unknown_50h;
		uint32_t Unknown_54h;
		uint32_t Unknown_58h;
		uint32_t Unknown_5Ch;
		uint32_t Unknown_60h;
		uint32_t Unknown_64h;
		uint32_t Unknown_68h;
		uint32_t Unknown_6Ch;
	} Meta_struct;

	std::vector<MetaStructureInfo> MetaInfos;
	std::vector<MetaEnumInfo> EnumInfos;
	std::vector<MetaDataBlock> MetaBlocks;

	Meta(memstream& file) {
		file.read((char*)&Meta_struct, sizeof(Meta_struct));

		if ((Meta_struct.StructureInfosPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			Meta_struct.StructureInfosPointer = Meta_struct.StructureInfosPointer & ~0x50000000;
		}
		if ((Meta_struct.StructureInfosPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			Meta_struct.StructureInfosPointer = Meta_struct.StructureInfosPointer & ~0x60000000;
		}

		file.seekg(Meta_struct.StructureInfosPointer);

		for (int i = 0; i < Meta_struct.StructureInfosCount; i++)
		{
			MetaStructureInfo info(file);
			MetaInfos.push_back(info);
		}

		if ((Meta_struct.EnumInfosPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			Meta_struct.EnumInfosPointer = Meta_struct.EnumInfosPointer & ~0x50000000;
		}
		if ((Meta_struct.EnumInfosPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			Meta_struct.EnumInfosPointer = Meta_struct.EnumInfosPointer & ~0x60000000;
		}

		file.seekg(Meta_struct.EnumInfosPointer);

		for (size_t i = 0; i < Meta_struct.EnumInfosCount; i++)
		{
			MetaEnumInfo info(file);
			EnumInfos.push_back(info);
		}

		if ((Meta_struct.DataBlocksPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			Meta_struct.DataBlocksPointer = Meta_struct.DataBlocksPointer & ~0x50000000;
		}
		if ((Meta_struct.DataBlocksPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			Meta_struct.DataBlocksPointer = Meta_struct.DataBlocksPointer & ~0x60000000;
		}

		file.seekg(Meta_struct.DataBlocksPointer);

		for (int i = 0; i < Meta_struct.DataBlocksCount; i++)
		{
			MetaDataBlock block(file);
			MetaBlocks.push_back(block);
		}
	}

};

YmapLoader::YmapLoader(memstream& file)
{
/*	//ADD NEED
	uint32_t rsc7;
	file.read((char*)&rsc7, sizeof(uint32_t));

	if (rsc7 == 0x37435352) {
		printf("NOT IMPLEMENTED!");
	}
	else {
		printf("HERE");
	}*/

	struct {
		uint32_t FileVFT;
		uint32_t FileUnknown;
		uint64_t FilePagesInfoPointer;
	} ResourceFileBase;
	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

	///YMAP REAL DATA

	struct Array_Structure //16 bytes - pointer for a structure array
	{
		uint32_t Pointer;
		uint32_t Unk0;
		uint16_t Count1;
		uint16_t Count2;
		uint32_t Unk1;
	};
	struct Array_uint //16 bytes - pointer for a uint array
	{
		uint32_t Pointer;
		uint32_t Unk0;
		uint16_t Count1;
		uint16_t Count2;
		uint32_t Unk1;
	};
	struct Array_float //16 bytes - pointer for a float array
	{
		uint32_t Pointer;
		uint32_t Unk0;
		uint16_t Count1;
		uint16_t Count2;
		uint32_t Unk1;
	};
	struct Array_byte //16 bytes - pointer for a byte array
	{
		uint32_t Pointer;
		uint32_t Unk0;
		uint16_t Count1;
		uint16_t Count2;
		uint32_t Unk1;
	};
	struct CharPointer //16 bytes - pointer for a char array
	{
		uint32_t Pointer;
		uint32_t Unk0;
		uint16_t Count1;
		uint16_t Count2;
		uint32_t Unk1;
	};
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
	} _CMapData;


	Meta meta(file);

	for (auto& Block : meta.MetaBlocks)
	{
		if (Block.MetaDataBlock_struct.StructureNameHash == 3545841574)
		{
			std::memcpy(&_CMapData, Block.Data, sizeof(CMapData));
			//_CMapData = (CMapData)Block.Data[0];
			//MapData _
			//MapData = (MapData)Block.Data;
		}
	}

	for (auto& Block : meta.MetaBlocks)
	{
		if (Block.MetaDataBlock_struct.StructureNameHash == 3461354627)
		{
			//printf("WWTF\n");
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CEntityDef); i++)
			{
				CEntityDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CEntityDef)], sizeof(CEntityDef));
				CEntityDefs.push_back(def);
			}
		}
	}
}


YmapLoader::~YmapLoader()
{
}
