#pragma once
#include "Mesh.h"
#include "fstream"
#include <iostream>
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "membuf.h"
#include "nv_dds.h"

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

class YtdFile
{
public:
	uint32_t Hash;
	std::vector<GLuint> textures;

	YtdFile(memstream& file, uint32_t hash);
	~YtdFile();
};

