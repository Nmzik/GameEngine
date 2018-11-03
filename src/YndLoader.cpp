#include "YndLoader.h"

YndLoader::YndLoader(memstream& file)
{
	/*file.read((char*)&NodeDictionary, sizeof(NodeDictionary));

	TranslatePTR(NodeDictionary.NodesPointer);

	file.seekg(NodeDictionary.NodesPointer);
	nodes.resize(NodeDictionary.NodesCount);

	for (uint32_t i = 0; i < NodeDictionary.NodesCount; i++)
	{
		file.read((char*)&nodes[i], sizeof(Node));
	}

	////////////////
	TranslatePTR(NodeDictionary.LinksPtr);

	file.seekg(NodeDictionary.LinksPtr);
	nodeLinks.resize(NodeDictionary.LinksCount);

	for (uint32_t i = 0; i < NodeDictionary.LinksCount; i++)
	{
		file.read((char*)&nodeLinks[i], sizeof(NodeLink));
	}

	/////////////////
	TranslatePTR(NodeDictionary.JunctionsPtr);

	file.seekg(NodeDictionary.JunctionsPtr);
	nodeJunctions.resize(NodeDictionary.JunctionsCount);

	for (uint32_t i = 0; i < NodeDictionary.JunctionsCount; i++)
	{
		file.read((char*)&nodeJunctions[i], sizeof(NodeJunction));
	}

	//////////////////
	TranslatePTR(NodeDictionary.JunctionHeightmapBytesPtr);

	file.seekg(NodeDictionary.JunctionHeightmapBytesPtr);
	JunctionHeightmapBytes.resize(NodeDictionary.JunctionHeightmapBytesCount);
	file.read((char*)&JunctionHeightmapBytes[0], NodeDictionary.JunctionHeightmapBytesCount);

	///////////////////
	TranslatePTR(NodeDictionary.JunctionRefsPtr);

	file.seekg(NodeDictionary.JunctionRefsPtr);
	NodeJunctionRefs.resize(NodeDictionary.JunctionRefsCount1);

	for (int i = 0; i < NodeDictionary.JunctionRefsCount1; i++)
	{
		file.read((char*)&NodeJunctionRefs[i], sizeof(NodeJunctionRef));
	}*/
}


YndLoader::~YndLoader()
{
}
