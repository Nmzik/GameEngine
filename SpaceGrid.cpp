#include "SpaceGrid.h"



SpaceGrid::SpaceGrid()
{
	Grid = new SpaceGridCell[500*500];
	/*for (int i = 0; i < 500; i++)
	{
		Grid[i] = new SpaceGridCell[500];
	}*/
}


SpaceGrid::~SpaceGrid()
{
}

void SpaceGrid::AddBoundsItem(BoundsStoreItem* item)
{
	glm::vec2 min = GetCellPos(item->Min);
	glm::vec2 max = GetCellPos(item->Max);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			if (!((x < 0) || (y < 0) || (x >= CellCount) || (y >= CellCount)))
				Grid[x * 500 + y].AddBoundsItem(item);
				//Grid[x][y].AddBoundsItem(item);
			/*SpaceGridCell* cell = GetCell(glm::vec2(x, y));
			if (cell != nullptr)
			{
			cell->AddNode(node);
			}*/
		}
	}
}

void SpaceGrid::AddMapNode(MapDataStoreNode* node)
{
	bool useouter = true;

	glm::vec2 min = GetCellPos(useouter ? node->streamingExtentsMin : node->entitiesExtentsMin);
	glm::vec2 max = GetCellPos(useouter ? node->streamingExtentsMax : node->entitiesExtentsMax);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			if (!((x < 0) || (y < 0) || (x >= CellCount) || (y >= CellCount)))
				Grid[x * 500 + y].AddNode(node);
				//Grid[x][y].AddNode(node);
			/*SpaceGridCell* cell = GetCell(glm::vec2(x, y));
			if (cell != nullptr)
			{
				cell->AddNode(node);
			}*/
		}
	}
}

SpaceGridCell* SpaceGrid::GetCell(glm::vec3 P)
{
	return GetCell(GetCellPos(P));
}
typedef SpaceGridCell test[][500];

SpaceGridCell* SpaceGrid::GetCell(glm::i32vec2 g)
{
	if ((g.x < 0) || (g.y < 0) || (g.x >= CellCount) || (g.y >= CellCount))
	{
		printf("ERROR HERE\n");
		return new SpaceGridCell();
	}
	return &Grid[g.x * 500 + g.y];
	/*const SpaceGridCell cell = Grid[g.x, g.y];
	Grid[300][250]
	if (cell == null)
	{
		cell = new SpaceGridCell();
		Cells[g.X, g.Y] = cell;
	}
	return cell;*/
}

glm::i32vec2 SpaceGrid::GetCellPos(glm::vec3 p)
{
	glm::vec3 ind = (p + WorldSize) * CellSizeInv;
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
