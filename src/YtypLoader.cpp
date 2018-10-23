#include "YtypLoader.h"

YtypLoader::YtypLoader(memstream2& file)
{
	Meta meta(file);

	CMapTypes _CMapTypes;

	for (auto& Block : meta.MetaBlocks)
	{
		if (Block.MetaDataBlock_struct->StructureNameHash == 3649811809)
		{
			std::memcpy(&_CMapTypes, &file.data[Block.MetaDataBlock_struct->DataPointer], sizeof(CMapTypes));
		}

		else if (Block.MetaDataBlock_struct->StructureNameHash == 2195127427) //CBaseArchetypeDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(CBaseArchetypeDef); i++)
			{
				CBaseArchetypeDef def;
				std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(CBaseArchetypeDef)], sizeof(CBaseArchetypeDef));
				CBaseArchetypeDefs.push_back(def);
			}
		}

		else if (Block.MetaDataBlock_struct->StructureNameHash == 1991296364) //CTimeArchetypeDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(CTimeArchetypeDef); i++)
			{
				CTimeArchetypeDef def;
				std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(CTimeArchetypeDef)], sizeof(CTimeArchetypeDef));
				CTimeArchetypeDefs.push_back(def);
			}
		}

		else if (Block.MetaDataBlock_struct->StructureNameHash == 273704021) //CMloArchetypeDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(CMloArchetypeDef); i++)
			{
				CMloArchetypeDef def;
				std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(CMloArchetypeDef)], sizeof(CMloArchetypeDef));
				CMloArchetypeDefs.push_back(def);
			}
		}

		/*if (Block.MetaDataBlock_struct.StructureNameHash == 3461354627)
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CEntityDef); i++)
			{
				CEntityDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CEntityDef)], sizeof(CEntityDef));
				CEntityDefs.push_back(def);
			}
		}*/
	}
}


YtypLoader::~YtypLoader()
{
}
