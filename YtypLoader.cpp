#include "YtypLoader.h"



YtypLoader::YtypLoader(memstream& file)
{
	struct {
		uint32_t FileVFT;
		uint32_t FileUnknown;
		uint64_t FilePagesInfoPointer;
	} ResourceFileBase;
	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

	Meta meta(file);

	struct CMapTypes //80 bytes, Key:2608875220
	{
		uint32_t Unused0;//0
		uint32_t Unused1;//4
		Array_StructurePointer extensions; //8   8: Array: 0: extensions  {0: StructurePointer: 0: 256}
		Array_StructurePointer archetypes; //24   24: Array: 0: archetypes  {0: StructurePointer: 0: 256}
		uint32_t name; //40   40: Hash: 0: name
		uint32_t Unused2;//44
		Array_uint dependencies; //48   48: Array: 0: dependencies//1013942340  {0: Hash: 0: 256}
		Array_Structure compositeEntityTypes;
	} _CMapTypes;

	for (auto& Block : meta.MetaBlocks)
	{
		if (Block.MetaDataBlock_struct.StructureNameHash == 3649811809)
		{
			std::memcpy(&_CMapTypes, Block.Data, sizeof(CMapTypes));
		}
	}

	for (auto& Block : meta.MetaBlocks)
	{
		if (Block.MetaDataBlock_struct.StructureNameHash == 2195127427)
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CBaseArchetypeDef); i++)
			{
				CBaseArchetypeDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CBaseArchetypeDef)], sizeof(CBaseArchetypeDef));
				CBaseArchetypeDefs.push_back(def);
			}
		}
	}
	//CELANING
	for (auto& Block : meta.MetaBlocks) {
		delete[] Block.Data;
	}

}


YtypLoader::~YtypLoader()
{
}