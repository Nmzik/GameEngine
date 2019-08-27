#include "YtypLoader.h"
#include "Meta.h"

YtypLoader::YtypLoader(memstream& file)
{
    Meta* meta = (Meta*)file.read(sizeof(Meta));
    meta->Resolve(file);

    for (int j = 0; j < meta->DataBlocksCount; j++)
    {
        switch (meta->DataBlocks[j].StructureNameHash)
        {
            case 3649811809:
            {
                CMapTypes _CMapTypes;
                std::memcpy(&_CMapTypes, &file.data[meta->DataBlocks[j].DataPointer], sizeof(CMapTypes));
                archetypeDefs.reserve(_CMapTypes.archetypes.Count1);
                break;
            }
            case 2195127427:
            {
                fwArchetypeDef* defs = (fwArchetypeDef*)&file.data[meta->DataBlocks[j].DataPointer];
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(fwArchetypeDef); i++)
                {
                    fwArchetype* arch = new fwArchetype();
                    arch->BaseArchetypeDef = defs[i];
                    archetypeDefs.push_back(arch);
                }

                break;
            }
            case 1991296364:
            {
                CTimeArchetypeDef* defs = (CTimeArchetypeDef*)&file.data[meta->DataBlocks[j].DataPointer];
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(CTimeArchetypeDef); i++)
                {
                    TimeArchetype* arch = new TimeArchetype();
                    arch->BaseArchetypeDef = defs[i]._BaseArchetypeDef;
                    arch->TimeArchetypeDef = defs[i]._TimeArchetypeDef;
                    archetypeDefs.push_back(arch);
                }

                break;
            }
            case 273704021:
            {
                CMloArchetypeDef* defs = (CMloArchetypeDef*)&file.data[meta->DataBlocks[j].DataPointer];
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(CMloArchetypeDef); i++)
                {
                    MloArchetype* arch = new MloArchetype();
                    arch->BaseArchetypeDef = defs[i]._BaseArchetypeDef;
                    arch->MloArchetypeDef = defs[i]._MloArchetypeDefData;
                    archetypeDefs.push_back(arch);
                }

                break;
            }
            case 3461354627:
            {
                fwEntityDef* defs = (fwEntityDef*)&file.data[meta->DataBlocks[j].DataPointer];
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(fwEntityDef); i++)
                {
                    fwEntityDefs.push_back(defs[i]);
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
