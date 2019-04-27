#include "YtypLoader.h"
#include "Meta.h"

YtypLoader::YtypLoader(memstream& file)
{
    Meta meta(file);

    for (auto& Block : meta.MetaBlocks)
    {
        switch (Block.MetaDataBlock_struct->StructureNameHash)
        {
            case 3649811809:
            {
                CMapTypes _CMapTypes;
                std::memcpy(&_CMapTypes, &file.data[Block.MetaDataBlock_struct->DataPointer], sizeof(CMapTypes));
                ArchetypeDefs.reserve(_CMapTypes.archetypes.Count1);
                break;
            }
            case 2195127427:
            {
                for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(fwArchetypeDef); i++)
                {
                    fwArchetypeDef def;
                    std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(fwArchetypeDef)], sizeof(fwArchetypeDef));

                    fwArchetype* arch = new fwArchetype();
                    arch->BaseArchetypeDef = def;
                    ArchetypeDefs.push_back(arch);
                }

                break;
            }
            case 1991296364:
            {
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
            case 273704021:
            {
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
            case 3461354627:
            {
                for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(fwEntityDef); i++)
                {
                    fwEntityDef def;
                    std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(fwEntityDef)], sizeof(fwEntityDef));
                    fwEntityDefs.push_back(def);
                }
            }
            break;
            default:
                break;
        }
    }
}

YtypLoader::~YtypLoader()
{
}
