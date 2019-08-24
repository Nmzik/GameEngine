#pragma once
#include "loaders/CacheDatFile.h"
#include <glm/gtc/type_precision.hpp>

class YndLoader;
class YnvLoader;

class SpaceGrid
{
private:
    const int CellCount = 500;
    const int LastCell = CellCount - 1;
    const float WorldSize = 10000.0f;                            // max world grid size +/- 10000 units
    const float CellSize = 2.0f * WorldSize / (float)CellCount;  // 20.0f; //size of a cell
    const float CellSizeInv = 1.0f / CellSize;                   // inverse of the cell size.
    const float CellSizeHalf = CellSize * 0.5f;

public:
    SpaceGrid() = default;
    ~SpaceGrid() = default;

    glm::i32vec2 getCellPos(glm::vec3 p)
    {
        glm::i32vec2 ind = (p + WorldSize) * CellSizeInv;
        int x = (int)ind.x;
        int y = (int)ind.y;
        x = (x < 0) ? 0 : (x > LastCell) ? LastCell : x;
        y = (y < 0) ? 0 : (y > LastCell) ? LastCell : y;
        return glm::i32vec2(x, y);
    }
};

class NodeGrid
{
private:
    const float CellSize = 512.0f;
    const float CellSizeInv = 1.0f / CellSize;  //	inverse of the cell size.
    const int CellCountX = 32;
    const int CellCountY = 32;
    const float CornerX = -8192.0f;
    const float CornerY = -8192.0f;

public:
    NodeGrid() = default;
    ~NodeGrid() = default;

    glm::i32vec2 getCellPos(glm::vec3 p)
    {
        glm::vec3 ind = (p - glm::vec3(CornerX, CornerY, 0)) * CellSizeInv;
        int x = (int)ind.x;
        int y = (int)ind.y;
        x = (x < 0) ? 0 : (x >= CellCountX) ? CellCountX - 1 : x;
        y = (y < 0) ? 0 : (y >= CellCountY) ? CellCountY - 1 : y;
        return glm::i32vec2(x, y);
    }
};

class NavGrid
{
private:
    float CellSize = 150.0f;
    float CellSizeInv;  //	inverse of the cell size.
    int CellCountX = 100;
    int CellCountY = 100;
    float CornerX = -6000.0f;  // max = -6000+(100*150) = 9000
    float CornerY = -6000.0f;

public:
    NavGrid() = default;
    ~NavGrid() = default;

    glm::i32vec2 getCellPos(glm::vec3 p)
    {
        glm::vec3 ind = (p - glm::vec3(CornerX, CornerY, 0)) * CellSizeInv;
        int x = (int)ind.x;
        int y = (int)ind.y;
        x = (x < 0) ? 0 : (x >= CellCountX) ? CellCountX - 1 : x;
        y = (y < 0) ? 0 : (y >= CellCountY) ? CellCountY - 1 : y;
        return glm::i32vec2(x, y);
    }
};

/*
#pragma once
#include "CacheDatFile.h"

class YndLoader;
class YnvLoader;

class SpaceGridCell
{
 public:
 std::vector<uint16_t> MapNodes;
 std::vector<uint16_t> CInteriorProxies;
 std::vector<uint16_t> BoundsStoreItems;

 SpaceGridCell();
 ~SpaceGridCell();

 void AddNode(uint16_t node)
 {
  MapNodes.emplace_back(node);
 }

 void AddBoundsItem(uint16_t item)
 {
  BoundsStoreItems.emplace_back(item);
 }

 void AddCInteriorProxy(uint16_t proxy)
 {
  CInteriorProxies.emplace_back(proxy);
 }
};

class SpaceGrid
{
 private:
 const int CellCount      = 500;
 const int LastCell       = CellCount - 1;
 const float WorldSize    = 10000.0f;                            // max world grid size +/- 10000 units
 const float CellSize     = 2.0f * WorldSize / (float)CellCount; // 20.0f; //size of a cell
 const float CellSizeInv  = 1.0f / CellSize;                     // inverse of the cell size.
 const float CellSizeHalf = CellSize * 0.5f;
 SpaceGridCell Grid[500 * 500];

 public:
 SpaceGrid();
 ~SpaceGrid();

 void AddBoundsItem(BoundsStoreItem* item, uint16_t id);
 void AddMapNode(MapDataStoreNode* node, uint16_t id);
 void AddCInteriorProxy(CInteriorProxy* proxy, uint16_t id);
 SpaceGridCell& GetCell(glm::vec3 P);
 SpaceGridCell& GetCell(glm::i32vec2 g);
 glm::i32vec2 GetCellPos(glm::vec3 p);
};

class NodeGridCell
{
 public:
 int X;
 int Y;
 int ID;
 YndLoader* ynd = nullptr;

 NodeGridCell()
 {
 }

 void InitializeCell(int x, int y)
 {
  X  = x;
  Y  = y;
  ID = y * 32 + x;
 }
};

class NodeGrid
{
 private:
 const float CellSize    = 512.0f;
 const float CellSizeInv = 1.0f / CellSize; //	inverse of the cell size.
 const int CellCountX    = 32;
 const int CellCountY    = 32;
 const float CornerX     = -8192.0f;
 const float CornerY     = -8192.0f;

 NodeGridCell cells[32 * 32];

 public:
 NodeGrid()
 {
  for (int x = 0; x < CellCountX; x++)
  {
   for (int y = 0; y < CellCountY; y++)
   {
    cells[x * CellCountX + y].InitializeCell(x, y);
   }
  }
 }

 ~NodeGrid()
 {
 }

 glm::i32vec2 GetCellPos(glm::vec3 p)
 {
  glm::vec3 ind = (p - glm::vec3(CornerX, CornerY, 0)) * CellSizeInv;
  int x         = (int)ind.x;
  int y         = (int)ind.y;
  x             = (x < 0) ? 0 : (x >= CellCountX) ? CellCountX - 1 : x;
  y             = (y < 0) ? 0 : (y >= CellCountY) ? CellCountY - 1 : y;
  return glm::i32vec2(x, y);
 }

 NodeGridCell& GetCell(glm::i32vec2 pos)
 {
  return cells[pos.x * 32 + pos.y];
 }
};

class SpaceNavGridCell
{
 YnvLoader* ynv;
};

class NavGrid
{
 private:
 SpaceNavGridCell Cells[100 * 100];
 float CellSize = 150.0f;
 float CellSizeInv; //	inverse of the cell size.
 int CellCountX = 100;
 int CellCountY = 100;
 float CornerX  = -6000.0f; // max = -6000+(100*150) = 9000
 float CornerY  = -6000.0f;

 public:
 glm::i32vec2 GetCellPos(glm::vec3 p)
 {
  glm::vec3 ind = (p - glm::vec3(CornerX, CornerY, 0)) * CellSizeInv;
  int x         = (int)ind.x;
  int y         = (int)ind.y;
  x             = (x < 0) ? 0 : (x >= CellCountX) ? CellCountX - 1 : x;
  y             = (y < 0) ? 0 : (y >= CellCountY) ? CellCountY - 1 : y;
  return glm::i32vec2(x, y);
 }
 SpaceNavGridCell& GetCell(glm::i32vec2 g)
 {
  return Cells[g.x * 100 + g.y];
 }
};

*/
