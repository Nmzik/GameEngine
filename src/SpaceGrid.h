#pragma once
#include <array>
#include <limits>
#include <optional>

#include "loaders/CacheDatFile.h"
#include <glm/gtc/type_precision.hpp>
#include <glm/vec3.hpp>

class YndLoader;
class YnvLoader;

class SpaceMapDataStoreNode
{
    std::optional<SpaceMapDataStoreNode>* childrens;
    std::vector<MapDataStoreNode> items;

    int depth;
    glm::vec3 BBmin;
    glm::vec3 BBmax;

public:
    SpaceMapDataStoreNode()
        : childrens(new std::optional<SpaceMapDataStoreNode>[4]())
        , depth(0)
        , BBmin(std::numeric_limits<float>::max())
        , BBmax(std::numeric_limits<float>::lowest())
    {
    }

    void addMapNode(MapDataStoreNode& node)
    {
        BBmin = glm::min(BBmin, node.streamingExtentsMin);
        BBmax = glm::max(BBmax, node.streamingExtentsMax);

        items.push_back(node);
    }

    void trySplit(int threshold)
    {
        if (items.size() <= threshold)
            return;

        std::vector<MapDataStoreNode> newItems;

        glm::vec3 ncen = (BBmax + BBmin) * 0.5f;
        glm::vec3 next = (BBmax - BBmin) * 0.5f;
        float nsiz = glm::max(next.x, next.y);
        float nsizh = nsiz * 0.5f;

        for (int i = 0; i < items.size(); i++)
        {
            glm::vec3 imin = items[i].streamingExtentsMin;
            glm::vec3 imax = items[i].streamingExtentsMax;
            glm::vec3 icen = (imax + imin) * 0.5f;
            glm::vec3 iext = (imax - imin) * 0.5f;
            float isiz = glm::max(iext.x, iext.y);

            if (isiz >= nsizh)
            {
                newItems.push_back(items[i]);
            }
            else
            {
                int cind = ((icen.x > ncen.x) ? 1 : 0) + ((icen.y > ncen.y) ? 2 : 0);

                if (!childrens[cind].has_value())
                {
                    childrens[cind] = std::make_optional<SpaceMapDataStoreNode>();
                    childrens[cind]->depth = depth + 1;
                }
                childrens[cind]->addMapNode(items[i]);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            if (childrens[i].has_value())
            {
                childrens[i]->trySplit(threshold);
            }
        }

        items = std::move(newItems);
    }

    void getItems(std::vector<MapDataStoreNode>& nodes, glm::vec3& pos)
    {
        if ((pos.x >= BBmin.x) && (pos.x <= BBmax.x) && (pos.y >= BBmin.y) && (pos.y <= BBmax.y))
        {
            for (int i = 0; i < items.size(); i++)
            {
                glm::vec3 imin = items[i].streamingExtentsMin;
                glm::vec3 imax = items[i].streamingExtentsMax;
                if ((pos.x >= imin.x) && (pos.x <= imax.x) && (pos.y >= imin.y) && (pos.y <= imax.y))
                {
                    nodes.push_back(items[i]);
                }
            }
            for (int i = 0; i < 4; i++)
            {
                if (childrens[i].has_value())
                {
                    childrens[i]->getItems(nodes, pos);
                }
            }
        }
    }
};

class SpaceBoundsStoreNode
{
    std::optional<SpaceBoundsStoreNode>* childrens;
    std::vector<BoundsStoreItem> items;

    int depth;
    glm::vec3 BBmin;
    glm::vec3 BBmax;

public:
    SpaceBoundsStoreNode()
        : childrens(new std::optional<SpaceBoundsStoreNode>[4]())
        , depth(0)
        , BBmin(std::numeric_limits<float>::max())
        , BBmax(std::numeric_limits<float>::lowest())
    {
    }

    void addBoundsNode(BoundsStoreItem& node)
    {
        BBmin = glm::min(BBmin, node.Min);
        BBmax = glm::max(BBmax, node.Max);

        items.push_back(node);
    }

    void trySplit(int threshold)
    {
        if (items.size() <= threshold)
            return;

        std::vector<BoundsStoreItem> newItems;

        glm::vec3 ncen = (BBmax + BBmin) * 0.5f;
        glm::vec3 next = (BBmax - BBmin) * 0.5f;
        float nsiz = glm::max(next.x, next.y);
        float nsizh = nsiz * 0.5f;

        for (int i = 0; i < items.size(); i++)
        {
            glm::vec3 imin = items[i].Min;
            glm::vec3 imax = items[i].Max;
            glm::vec3 icen = (imax + imin) * 0.5f;
            glm::vec3 iext = (imax - imin) * 0.5f;
            float isiz = glm::max(iext.x, iext.y);

            if (isiz >= nsizh)
            {
                newItems.push_back(items[i]);
            }
            else
            {
                int cind = ((icen.x > ncen.x) ? 1 : 0) + ((icen.y > ncen.y) ? 2 : 0);
                if (!childrens[cind].has_value())
                {
                    childrens[cind] = std::make_optional<SpaceBoundsStoreNode>();
                    childrens[cind]->depth = depth + 1;
                }
                childrens[cind]->addBoundsNode(items[i]);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            if (childrens[i].has_value())
            {
                childrens[i]->trySplit(threshold);
            }
        }

        items = std::move(newItems);
    }

    void getItems(std::vector<BoundsStoreItem>& nodes, glm::vec3& pos)
    {
        if ((pos.x >= BBmin.x) && (pos.x <= BBmax.x) && (pos.y >= BBmin.y) && (pos.y <= BBmax.y))
        {
            for (int i = 0; i < items.size(); i++)
            {
                glm::vec3 imin = items[i].Min;
                glm::vec3 imax = items[i].Max;
                if ((pos.x >= imin.x) && (pos.x <= imax.x) && (pos.y >= imin.y) && (pos.y <= imax.y))
                {
                    nodes.push_back(items[i]);
                }
            }
            for (int i = 0; i < 4; i++)
            {
                if (childrens[i].has_value())
                {
                    childrens[i]->getItems(nodes, pos);
                }
            }
        }
    }
};

class SpaceMapDataStore
{
    SpaceMapDataStoreNode rootNode;
    const int splitThreshold = 10;

public:
    void Init(std::vector<MapDataStoreNode>& mapNodes)
    {
        for (int i = 0; i < mapNodes.size(); i++)
        {
            rootNode.addMapNode(mapNodes[i]);
        }
        rootNode.trySplit(splitThreshold);
    }

    void GetItems(std::vector<MapDataStoreNode>& nodes, glm::vec3& pos)
    {
        rootNode.getItems(nodes, pos);
    }
};

class SpaceBoundsStore
{
    SpaceBoundsStoreNode rootNode;
    const int splitThreshold = 10;

public:
    void Init(std::vector<BoundsStoreItem>& boundsNodes)
    {
        for (int i = 0; i < boundsNodes.size(); i++)
        {
            rootNode.addBoundsNode(boundsNodes[i]);
        }
        rootNode.trySplit(splitThreshold);
    }

    void GetItems(std::vector<BoundsStoreItem>& nodes, glm::vec3& pos)
    {
        rootNode.getItems(nodes, pos);
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
