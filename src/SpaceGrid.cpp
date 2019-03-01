#include "SpaceGrid.h"

#include "CacheDatFile.h"

/*void SpaceGrid::AddBoundsItem(BoundsStoreItem* item, uint16_t id)
{
	glm::i32vec2 min = GetCellPos(item->Min);
	glm::i32vec2 max = GetCellPos(item->Max);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			if (!((x < 0) || (y < 0) || (x >= CellCount) || (y >= CellCount)))
				Grid[x * 500 + y].AddBoundsItem(id);
		}
	}
}

void SpaceGrid::AddMapNode(MapDataStoreNode* node, uint16_t id)
{
	glm::i32vec2 min = GetCellPos(node->streamingExtentsMin); //	NOT entitiesExtents !!!! GIVES WEIRD RESULTS
	glm::i32vec2 max = GetCellPos(node->streamingExtentsMax);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			if (!((x < 0) || (y < 0) || (x >= CellCount) || (y >= CellCount)))
				Grid[x * 500 + y].AddNode(id);
		}
	}
}

void SpaceGrid::AddCInteriorProxy(CInteriorProxy* proxy, uint16_t id)
{
	glm::i32vec2 min = GetCellPos(proxy->BBMin);
	glm::i32vec2 max = GetCellPos(proxy->BBMax);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			if (!((x < 0) || (y < 0) || (x >= CellCount) || (y >= CellCount)))
				Grid[x * 500 + y].AddCInteriorProxy(id);
		}
	}
}*/

/*SpaceGridCell& SpaceGrid::GetCell(glm::vec3 P)
{
	return GetCell(GetCellPos(P));
}

SpaceGridCell& SpaceGrid::GetCell(glm::i32vec2 g)
{
	/*if ((g.x < 0) || (g.y < 0) || (g.x >= CellCount) || (g.y >= CellCount))
	{
	 printf("ERROR HERE\n");
	 return Grid[0];
	}*/
	/*return Grid[g.x * 500 + g.y];
}*/