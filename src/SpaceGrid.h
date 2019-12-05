#pragma once
#include "loaders/CacheDatFile.h"
#include <glm/gtc/type_precision.hpp>
#include <glm/vec3.hpp>
#include <limits>

class YndLoader;
class YnvLoader;

class SpaceMapDataStoreNode {
    std::vector<SpaceMapDataStoreNode*> childrens;
    std::vector<MapDataStoreNode>* items;
    
    int depth;
    glm::vec3 BBmin;
    glm::vec3 BBmax;
public:
    SpaceMapDataStoreNode()
    : depth(0)
    , BBmin(std::numeric_limits<float>::max())
    , BBmax(std::numeric_limits<float>::min())
    , items(nullptr)
    {
    }
    
    void addMapNode(MapDataStoreNode& node) {
        BBmin = glm::min(BBmin, node.streamingExtentsMin);
        BBmax = glm::max(BBmax, node.streamingExtentsMax);
        
        if (items == nullptr)
            items = new std::vector<MapDataStoreNode>();
        
        items->push_back(node);
    }
    
    void trySplit(int threshold) {
        if ((items == nullptr) || (items->size() <= threshold))
            return;
        
        childrens.resize(4);
        
        std::vector<MapDataStoreNode>* newItems = new std::vector<MapDataStoreNode>();
        
        glm::vec3 ncen = (BBmax + BBmin) * 0.5f;
        glm::vec3 next = (BBmax - BBmin) * 0.5f;
        float nsiz = glm::max(next.x, next.y);
        float nsizh = nsiz * 0.5f;
        
        for (int i = 0; i < items->size(); i++) {
            glm::vec3 imin = (*items)[i].streamingExtentsMin;
            glm::vec3 imax = (*items)[i].streamingExtentsMax;
            glm::vec3 icen = (imax + imin) * 0.5f;
            glm::vec3 iext = (imax - imin) * 0.5f;
            float isiz = glm::max(iext.x, iext.y);

            if (isiz >= nsizh)
            {
                newItems->push_back((*items)[i]);
            }
            else
            {
                float cind = ((icen.x > ncen.x) ? 1 : 0) + ((icen.y > ncen.y) ? 2 : 0);
                if (childrens[cind] == nullptr)
                {
                    childrens[cind] = new SpaceMapDataStoreNode();
                    childrens[cind]->depth = depth + 1;
                }
                childrens[cind]->addMapNode((*items)[i]);
            }
        }
        
        for (int i = 0; i < 4; i++)
        {
            if (childrens[i] != nullptr)
            {
                childrens[i]->trySplit(threshold);
            }
        }
        
        items = newItems;
    }
};

class SpaceMapDataStore
{
    SpaceMapDataStoreNode rootNode;
    const int splitThreshold = 10;
public:
    void Init(std::vector<MapDataStoreNode>& mapNodes) {
        for (int i = 0; i < mapNodes.size(); i++) {
            rootNode.addMapNode(mapNodes[i]);
        }
        rootNode.trySplit(splitThreshold);
    }
};

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
