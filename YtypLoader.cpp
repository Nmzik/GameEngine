#include "YtypLoader.h"

YtypLoader::YtypLoader(memstream& file)
{
	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

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

		if (Block.MetaDataBlock_struct.StructureNameHash == 2195127427) //CBaseArchetypeDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CBaseArchetypeDef); i++)
			{
				CBaseArchetypeDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CBaseArchetypeDef)], sizeof(CBaseArchetypeDef));
				CBaseArchetypeDefs.push_back(def);
			}
		}

		if (Block.MetaDataBlock_struct.StructureNameHash == 1991296364) //CTimeArchetypeDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CTimeArchetypeDef); i++)
			{
				CTimeArchetypeDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CTimeArchetypeDef)], sizeof(CTimeArchetypeDef));
				CTimeArchetypeDefs.push_back(def);
			}
		}

		if (Block.MetaDataBlock_struct.StructureNameHash == 273704021) //CMloArchetypeDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CMloArchetypeDef); i++)
			{
				CMloArchetypeDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CMloArchetypeDef)], sizeof(CMloArchetypeDef));
				CMloArchetypeDefs.push_back(def);
			}
		}

		if (CMloArchetypeDefs.size() != 0) {
			for (auto& Block : meta.MetaBlocks)
			{
				if (Block.MetaDataBlock_struct.StructureNameHash == 3461354627)
				{
					for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CEntityDef); i++)
					{
						CEntityDef def;
						std::memcpy(&def, &Block.Data[i * sizeof(CEntityDef)], sizeof(CEntityDef));
						CEntityDefs.push_back(def);
					}
				}
			}
		}

	}
}


YtypLoader::~YtypLoader()
{
}
