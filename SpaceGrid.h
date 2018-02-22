#pragma once
#include "fstream"
#include <iostream>
#include <vector>
#include <string>
#include "membuf.h"
#include "zlib.h"
#include "glm.hpp"
#include "CacheDatFile.h"

class SpaceGridCell
{
public:
	std::vector<MapDataStoreNode*> MapNodes;
	std::vector<CInteriorProxy*> CInteriorProxies;
	std::vector<BoundsStoreItem*> BoundsStoreItems;

	SpaceGridCell();
	~SpaceGridCell();

	void AddNode(MapDataStoreNode* node) {
		MapNodes.emplace_back(node);
	}

	void AddBoundsItem(BoundsStoreItem* item) {
		BoundsStoreItems.emplace_back(item);
	}
};

class SpaceGrid
{
public:
	const int CellCount = 500;
	const int LastCell = CellCount - 1;
	const float WorldSize = 10000.0f; //max world grid size +/- 10000 units
	const float CellSize = 2.0f * WorldSize / (float)CellCount;//20.0f; //size of a cell
	const float CellSizeInv = 1.0f / CellSize; //inverse of the cell size.
	const float CellSizeHalf = CellSize * 0.5f;
	SpaceGridCell Grid[500 * 500];
	//std::array<

	SpaceGrid();
	~SpaceGrid();

	void AddBoundsItem(BoundsStoreItem* item);
	void AddMapNode(MapDataStoreNode* node);
	SpaceGridCell &GetCell(glm::vec3 P);
	SpaceGridCell &GetCell(glm::i32vec2 g);
	glm::i32vec2 GetCellPos(glm::vec3 p);
};

