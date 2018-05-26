#pragma once
#include "TextureManager.h"
#include "YdrLoader.h"

struct ResourceSimpleList64Ptr
{
	// structure data
	uint64_t EntriesPointer;
	uint16_t EntriesCount;
	uint16_t EntriesCapacity;
	uint32_t Unused1;
};

struct TextureDictionary {
	uint32_t Unknown_10h; // 0x00000000
	uint32_t Unknown_14h; // 0x00000000
	uint32_t Unknown_18h; // 0x00000001
	uint32_t Unknown_1Ch; // 0x00000000
	ResourceSimpleList64Ptr TextureNameHashesPtr;
};

struct TextureBase{
	// structure data
	uint32_t VFT;
	uint32_t Unknown_4h; // 0x00000001
	uint32_t Unknown_8h; // 0x00000000
	uint32_t Unknown_Ch; // 0x00000000
	uint32_t Unknown_10h; // 0x00000000
	uint32_t Unknown_14h; // 0x00000000
	uint32_t Unknown_18h; // 0x00000000
	uint32_t Unknown_1Ch; // 0x00000000
	uint32_t Unknown_20h; // 0x00000000
	uint32_t Unknown_24h; // 0x00000000
	uint64_t NamePointer;
	uint32_t Unknown_30h;
	uint32_t Unknown_34h; // 0x00000000
	uint32_t Unknown_38h; // 0x00000000
	uint32_t Unknown_3Ch; // 0x00000000
};

class YtdLoader
{
public:
	uint32_t time;
	std::vector<uint32_t> TextureNameHashes;

	void Init(memstream& file, int32_t systemSize);
	~YtdLoader();
};

