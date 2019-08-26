#include "YndLoader.h"

YndLoader::YndLoader(memstream& file)
{
    NodeDictionary nodeDictionary;
    file.read((char*)&nodeDictionary, sizeof(NodeDictionary));

    TranslatePTR(nodeDictionary.NodesPointer);

    file.seekg(nodeDictionary.NodesPointer);
    nodes.resize(nodeDictionary.NodesCount);

    for (uint32_t i = 0; i < nodeDictionary.NodesCount; i++)
    {
        file.read((char*)&nodes[i], sizeof(Node));
    }

    ////////////////
    TranslatePTR(nodeDictionary.LinksPtr);

    file.seekg(nodeDictionary.LinksPtr);
    nodeLinks.resize(nodeDictionary.LinksCount);

    for (uint32_t i = 0; i < nodeDictionary.LinksCount; i++)
    {
        file.read((char*)&nodeLinks[i], sizeof(NodeLink));
    }

    /////////////////
    TranslatePTR(nodeDictionary.JunctionsPtr);

    file.seekg(nodeDictionary.JunctionsPtr);
    nodeJunctions.resize(nodeDictionary.JunctionsCount);

    for (uint32_t i = 0; i < nodeDictionary.JunctionsCount; i++)
    {
        file.read((char*)&nodeJunctions[i], sizeof(NodeJunction));
    }

    //////////////////
    TranslatePTR(nodeDictionary.JunctionHeightmapBytesPtr);

    file.seekg(nodeDictionary.JunctionHeightmapBytesPtr);
    //junctionHeightmapBytes.resize(nodeDictionary.JunctionHeightmapBytesCount);
    //file.read((char*)&junctionHeightmapBytes[0], nodeDictionary.JunctionHeightmapBytesCount);

    ///////////////////
    TranslatePTR(nodeDictionary.JunctionRefsPtr);

    file.seekg(nodeDictionary.JunctionRefsPtr);
    nodeJunctionRefs.resize(nodeDictionary.JunctionRefsCount1);

    for (int i = 0; i < nodeDictionary.JunctionRefsCount1; i++)
    {
        file.read((char*)&nodeJunctionRefs[i], sizeof(NodeJunctionRef));
    }
}

YndLoader::~YndLoader()
{
}
