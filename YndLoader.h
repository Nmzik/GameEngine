#pragma once

#include "fstream"
#include <iostream>
#include <vector>
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"

class YndLoader
{
	struct {
		uint64_t NodesPointer;
		uint32_t NodesCount;
		uint32_t NodesCountVehicle;
		uint32_t NodesCountPed;
		uint32_t Unk24; // 0x00000000
		uint64_t LinksPtr;
		uint32_t LinksCount;
		uint32_t Unk34; // 0x00000000
		uint64_t JunctionsPtr;
		uint64_t JunctionHeightmapBytesPtr;
		uint32_t Unk48; // 0x00000001
		uint32_t Unk4C; // 0x00000000
		uint64_t JunctionRefsPtr;
		uint16_t JunctionRefsCount0;
		uint16_t JunctionRefsCount1; // same as JunctionRefsCount0
		uint32_t Unk5C; // 0x00000000
		uint32_t JunctionsCount; // same as JunctionRefsCount0
		uint32_t JunctionHeightmapBytesCount;
		uint32_t Unk68; // 0x00000000
		uint32_t Unk6C; // 0x00000000
	} NodeDictionary;

	struct Node {
		uint32_t Unused0; // 0x00000000
		uint32_t Unused1; // 0x00000000
		uint32_t Unused2; // 0x00000000
		uint32_t Unused3; // 0x00000000
		uint16_t AreaID;
		uint16_t NodeID;
		uint32_t StreetName; //TextHash 
		uint16_t Unused4;
		uint16_t LinkID;
		int16_t PositionX;
		int16_t PositionY;
		uint8_t Flags0;
		uint8_t Flags1;
		int16_t PositionZ;
		uint8_t Flags2;
		uint8_t LinkCountFlags;
		uint8_t Flags3;
		uint8_t Flags4;
	};

	struct NodeLink {
		uint16_t AreaID;
		uint16_t NodeID;
		uint8_t Flags0;
		uint8_t Flags1;
		uint8_t Flags2;
		uint8_t LinkLength;
	};

	struct NodeJunction {
		int16_t MaxZ;
		int16_t PositionX;
		int16_t PositionY;
		int16_t MinZ;
		uint16_t HeightmapPtr;
		int8_t HeightmapDimX;
		int8_t HeightmapDimY;
	};

	struct NodeJunctionRef {
		uint16_t AreaID;
		uint16_t NodeID;
		uint16_t JunctionID;
		uint16_t Unk0;

	};
public:
	std::vector<Node> nodes;
	std::vector<NodeLink> nodeLinks;
	std::vector<NodeJunction> nodeJunctions;
	std::vector<uint8_t> JunctionHeightmapBytes;
	std::vector<NodeJunctionRef> NodeJunctionRefs;

	YndLoader(std::string FileName);
	~YndLoader();
};

