#include "YmapLoader.h"

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

	uint64_t SYSTEM_BASE = 0x50000000;
	uint64_t GRAPHICS_BASE = 0x60000000;

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
	} Meta;

	struct MetaStructureInfo {
		uint32_t StructureNameHash;
		uint32_t StructureKey;
		uint32_t Unknown_8h;
		uint32_t Unknown_Ch; //0x00000000;
		uint64_t EntriesPointer;
		int32_t StructureSize;
		int16_t Unknown_1Ch; //0x0000;
		int16_t EntriesCount;
	};

	struct MetaStructureEntryInfo_s {
		uint32_t EntryNameHash;
		int32_t DataOffset;
		uint8_t DataType;
		uint8_t Unknown_9h;
		int16_t ReferenceTypeIndex;
		uint32_t ReferenceKey;
	};

	struct MetaEnumInfo {
		uint32_t EnumNameHash;
		uint32_t EnumKey;
		uint64_t EntriesPointer;
		int32_t EntriesCount;
		int32_t Unknown_14h; //0x00000000;
	};

	struct MetaEnumEntryInfo_s {
		uint32_t EntryNameHash;
		int32_t EntryValue;
	};

	struct MetaDataBlock {
		uint32_t StructureNameHash;
		int32_t DataLength;
		uint64_t DataPointer;
	};
	file.read((char*)&Meta, sizeof(Meta));
	
}


YmapLoader::~YmapLoader()
{
}
