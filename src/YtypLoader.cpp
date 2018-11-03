#include "YtypLoader.h"

YtypLoader::YtypLoader(memstream& file)
{
	Meta meta(file);

	CMapTypes _CMapTypes;

	for (auto& Block : meta.MetaBlocks)
	{
		switch (Block.MetaDataBlock_struct->StructureNameHash)
		{
			case 3649811809: {
				std::memcpy(&_CMapTypes, &file.data[Block.MetaDataBlock_struct->DataPointer], sizeof(CMapTypes));
				ArchetypeDefs.reserve(_CMapTypes.archetypes.Count1);
				break;
			}
			case 2195127427: {
				for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(CBaseArchetypeDef); i++)
				{
					CBaseArchetypeDef def;
					std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(CBaseArchetypeDef)], sizeof(CBaseArchetypeDef));

					Archetype* arch = new Archetype();
					arch->BaseArchetypeDef = def;
					ArchetypeDefs.push_back(arch);
				}

				break;
			}
			case 1991296364: {
				for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(CTimeArchetypeDef); i++)
				{
					CTimeArchetypeDef def;
					std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(CTimeArchetypeDef)], sizeof(CTimeArchetypeDef));

					TimeArchetype* arch = new TimeArchetype();
					arch->BaseArchetypeDef = def._BaseArchetypeDef;
					arch->TimeArchetypeDef = def._TimeArchetypeDef;
					ArchetypeDefs.push_back(arch);
				}

				break;
			}
			case 273704021: {
				for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(CMloArchetypeDef); i++)
				{
					CMloArchetypeDef def;
					std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(CMloArchetypeDef)], sizeof(CMloArchetypeDef));
					
					MloArchetype* arch = new MloArchetype();
					arch->BaseArchetypeDef = def._BaseArchetypeDef;
					arch->MloArchetypeDef = def._MloArchetypeDefData;
					ArchetypeDefs.push_back(arch);
				}

				break;
			}
			default:
				break;
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


YtypLoader::~YtypLoader()
{
}
