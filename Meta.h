#pragma once
#include "YdrLoader.h"

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

		uint64_t pos = file.tellg();

		file.seekg(MetaEnumInfo_struct.EntriesPointer);

		for (uint32_t i = 0; i < (uint32_t)MetaEnumInfo_struct.EntriesCount; i++)
		{
			MetaEnumEntryInfo_s info;
			file.read((char*)&info, sizeof(MetaEnumEntryInfo_s));
			infos.push_back(info);
		}
		file.seekg(pos);
	}
};

class MetaDataBlock {
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

	~Meta() {
		for (auto& Block : MetaBlocks) {
			delete[] Block.Data;
		}
	}

};