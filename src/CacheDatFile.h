#pragma once
#include "fstream"
#include <iostream>
#include <vector>
#include <string>
#include "membuf.h"
#include "glm/glm.hpp"

struct MapDataStoreNode
{
	uint32_t Name;
	uint32_t ParentName;
	uint32_t ContentFlags;
	glm::vec3 streamingExtentsMin;
	glm::vec3 streamingExtentsMax;
	glm::vec3 entitiesExtentsMin;
	glm::vec3 entitiesExtentsMax;
	uint8_t Unk1;
	uint8_t Unk2;
	uint8_t Unk3;
	uint8_t Unk4;
};

struct CInteriorProxy
{
	uint32_t Unk01;
	uint32_t Unk02;
	uint32_t Unk03;
	uint32_t Name;   //	YBN FILE NAME
	uint32_t Parent; //	YMAP FILE NAME
	glm::vec3 Position;
	glm::vec4 Orientation;
	glm::vec3 BBMin;
	glm::vec3 BBMax;
	float Unk11;
	uint32_t Unk12;
	float Unk13;
	uint32_t Unk14;
	float Unk15;
	uint32_t Unk16;
	uint32_t Unk17;
	uint32_t Unk18;
};
struct BoundsStoreItem
{
	uint32_t Name;
	glm::vec3 Min;
	glm::vec3 Max;
	uint32_t Layer;
};

class CacheDatFile
{
public:
	std::vector<MapDataStoreNode> AllMapNodes;
	std::vector<CInteriorProxy> AllCInteriorProxies;
	std::vector<BoundsStoreItem> AllBoundsStoreItems;

	CacheDatFile(std::vector<uint8_t>& Data);
	~CacheDatFile();
};
