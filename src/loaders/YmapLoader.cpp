#include "YmapLoader.h"
#include "Meta.h"
#include <btBulletDynamicsCommon.h>
#include "../CBuilding.h"

void YmapLoader::init(memstream& file)
{
    //	Could be an additional extraction code here
    Meta* meta = (Meta*)file.read(sizeof(Meta));
    meta->Resolve(file);

    //	FIND CMAPDATA FIRST OF ALL
    for (int j = 0; j < meta->DataBlocksCount; j++)
    {
        if (meta->DataBlocks[j].StructureNameHash == 3545841574)
        {
            CMapData* data = (CMapData*)&file.data[meta->DataBlocks[j].DataPointer];
            
            cMapData = *data;
            //	Optimization
            entities.reserve(cMapData.entities.Count1);
            carGenerators.reserve(cMapData.carGenerators.Count1);
        }
    }

    for (int j = 0; j < meta->DataBlocksCount; j++)
    {
        switch (meta->DataBlocks[j].StructureNameHash)
        {
            case 3461354627:
            {
                fwEntityDef* def = (fwEntityDef*)&file.data[meta->DataBlocks[j].DataPointer];
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(fwEntityDef); i++)
                {
                    def[i].rotation.w = -def[i].rotation.w;
                    //	if (def.lodLevel == Unk_1264241711::LODTYPES_DEPTH_ORPHANHD) def.lodDist *= 1.5f;
                    entities.emplace_back(def[i]);
                }
                break;
            }
            case 1860713439:
            {  //	CAR GENERATORS
                CCarGen* carGens = (CCarGen*)&file.data[meta->DataBlocks[j].DataPointer];
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(fwEntityDef); i++)
                {
                    carGenerators.push_back(carGens[i]);
                }
                break;
            }
            case 164374718:  //CMloInstanceDef
            {
                CMloInstanceDef* MloInstanceDef = (CMloInstanceDef*)&file.data[meta->DataBlocks[j].DataPointer];
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(CMloInstanceDef); i++)
                {
                    CMloInstanceDefs.push_back(MloInstanceDef[i]);
                }
                break;
            }
            default:
                break;
        }
    }
    /*for (int i = 0; i < Objects.size(); i++)
	{
		Object d = Objects[i];
		int pind = Objects[i].CEntity.parentIndex;

		bool isroot = false;

		if ((pind < 0) || (pind >= Objects.size()) || (pind >= i)) //index check? might be a problem
		{
		 isroot = true;
		}
		else
		{
		 Object p = Objects[pind];
		 if ((p.CEntity.lodLevel <= d.CEntity.lodLevel) ||
		  ((p.CEntity.lodLevel == Unk_1264241711::LODTYPES_DEPTH_ORPHANHD) &&
		  (d.CEntity.lodLevel != Unk_1264241711::LODTYPES_DEPTH_ORPHANHD)))
		 {
		  isroot = true;
		 }
		}

		if (isroot) {
		 RootObjects.push_back(Objects[i]);
		}
	}*/
}

YmapLoader::~YmapLoader()
{
}
