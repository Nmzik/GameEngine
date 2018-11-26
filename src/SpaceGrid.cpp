#include "SpaceGrid.h"

#include "CacheDatFile.h"

SpaceGrid::SpaceGrid()
{
	//	Grid = myNew SpaceGridCell[500*500];
}

SpaceGrid::~SpaceGrid()
{
}

void SpaceGrid::AddBoundsItem(BoundsStoreItem* item, uint16_t id)
{
	glm::vec2 min = GetCellPos(item->Min);
	glm::vec2 max = GetCellPos(item->Max);

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
	glm::vec2 min = GetCellPos(node->streamingExtentsMin); //	NOT entitiesExtents !!!! GIVES WEIRD RESULTS
	glm::vec2 max = GetCellPos(node->streamingExtentsMax);

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
	glm::vec2 min = GetCellPos(proxy->BBMin);
	glm::vec2 max = GetCellPos(proxy->BBMax);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			if (!((x < 0) || (y < 0) || (x >= CellCount) || (y >= CellCount)))
				Grid[x * 500 + y].AddCInteriorProxy(id);
		}
	}
}

SpaceGridCell& SpaceGrid::GetCell(glm::vec3 P)
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
	return Grid[g.x * 500 + g.y];
	/*const SpaceGridCell cell = Grid[g.x, g.y];
	Grid[300][250]
	if (cell == null)
	{
	 cell = myNew SpaceGridCell();
	 Cells[g.X, g.Y] = cell;
	}
	return cell;*/
}

glm::i32vec2 SpaceGrid::GetCellPos(glm::vec3 p)
{
	glm::i32vec2 ind = (p + WorldSize) * CellSizeInv;
	int x = (int)ind.x;
	int y = (int)ind.y;
	x = (x < 0) ? 0 : (x > LastCell) ? LastCell : x;
	y = (y < 0) ? 0 : (y > LastCell) ? LastCell : y;
	return glm::i32vec2(x, y);
}

SpaceGridCell::SpaceGridCell()
{
}

SpaceGridCell::~SpaceGridCell()
{
}
