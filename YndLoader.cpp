#include "YndLoader.h"

YndLoader::YndLoader(std::string FileName)
{
	std::ifstream file("C:\\Users\\nmzik\\Desktop\\test\\"+ FileName , std::ios::binary);

	if (!file.is_open()) {
		printf("NOT FOUND!");
	}

	struct {
		uint32_t FileVFT;
		uint32_t FileUnknown;
		uint64_t FilePagesInfoPointer;
	} ResourceFileBase;
	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

	///////////////////
	uint64_t SYSTEM_BASE = 0x50000000;
	uint64_t GRAPHICS_BASE = 0x60000000;

	file.read((char*)&NodeDictionary, sizeof(NodeDictionary));

	if ((NodeDictionary.NodesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		NodeDictionary.NodesPointer = NodeDictionary.NodesPointer & ~0x50000000;
	}
	if ((NodeDictionary.NodesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		NodeDictionary.NodesPointer = NodeDictionary.NodesPointer & ~0x60000000;
	}

	file.seekg(NodeDictionary.NodesPointer);
	nodes.resize(NodeDictionary.NodesCount);

	for (int i = 0; i < NodeDictionary.NodesCount; i++)
	{
		file.read((char*)&nodes[i], sizeof(Node));
	}

	////////////////
	if ((NodeDictionary.LinksPtr & SYSTEM_BASE) == SYSTEM_BASE) {
		NodeDictionary.LinksPtr = NodeDictionary.LinksPtr & ~0x50000000;
	}
	if ((NodeDictionary.LinksPtr & GRAPHICS_BASE) == GRAPHICS_BASE) {
		NodeDictionary.LinksPtr = NodeDictionary.LinksPtr & ~0x60000000;
	}

	file.seekg(NodeDictionary.LinksPtr);
	nodeLinks.resize(NodeDictionary.LinksCount);

	for (int i = 0; i < NodeDictionary.LinksCount; i++)
	{
		file.read((char*)&nodeLinks[i], sizeof(NodeLink));
	}

	/////////////////
	if ((NodeDictionary.JunctionsPtr & SYSTEM_BASE) == SYSTEM_BASE) {
		NodeDictionary.JunctionsPtr = NodeDictionary.JunctionsPtr & ~0x50000000;
	}
	if ((NodeDictionary.JunctionsPtr & GRAPHICS_BASE) == GRAPHICS_BASE) {
		NodeDictionary.JunctionsPtr = NodeDictionary.JunctionsPtr & ~0x60000000;
	}

	file.seekg(NodeDictionary.JunctionsPtr);
	nodeJunctions.resize(NodeDictionary.JunctionsCount);

	for (int i = 0; i < NodeDictionary.JunctionsCount; i++)
	{
		file.read((char*)&nodeJunctions[i], sizeof(NodeJunction));
	}

	//////////////////
	if ((NodeDictionary.JunctionHeightmapBytesPtr & SYSTEM_BASE) == SYSTEM_BASE) {
		NodeDictionary.JunctionHeightmapBytesPtr = NodeDictionary.JunctionHeightmapBytesPtr & ~0x50000000;
	}
	if ((NodeDictionary.JunctionHeightmapBytesPtr & GRAPHICS_BASE) == GRAPHICS_BASE) {
		NodeDictionary.JunctionHeightmapBytesPtr = NodeDictionary.JunctionHeightmapBytesPtr & ~0x60000000;
	}

	file.seekg(NodeDictionary.JunctionHeightmapBytesPtr);
	JunctionHeightmapBytes.resize(NodeDictionary.JunctionHeightmapBytesCount);
	file.read((char*)&JunctionHeightmapBytes[0], NodeDictionary.JunctionHeightmapBytesCount);

	///////////////////
	if ((NodeDictionary.JunctionRefsPtr & SYSTEM_BASE) == SYSTEM_BASE) {
		NodeDictionary.JunctionRefsPtr = NodeDictionary.JunctionRefsPtr & ~0x50000000;
	}
	if ((NodeDictionary.JunctionRefsPtr & GRAPHICS_BASE) == GRAPHICS_BASE) {
		NodeDictionary.JunctionRefsPtr = NodeDictionary.JunctionRefsPtr & ~0x60000000;
	}

	file.seekg(NodeDictionary.JunctionRefsPtr);
	NodeJunctionRefs.resize(NodeDictionary.JunctionRefsCount1);

	for (int i = 0; i < NodeDictionary.JunctionRefsCount1; i++)
	{
		file.read((char*)&NodeJunctionRefs[i], sizeof(NodeJunctionRef));
	}
}


YndLoader::~YndLoader()
{
}
