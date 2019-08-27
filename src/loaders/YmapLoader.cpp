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
            std::memcpy(&_CMapData, &file.data[meta->DataBlocks[j].DataPointer], sizeof(CMapData));

            //	Optimization
            entities.reserve(_CMapData.entities.Count1);
            carGenerators.reserve(_CMapData.carGenerators.Count1);
        }
    }

    for (int j = 0; j < meta->DataBlocksCount; j++)
    {
        switch (meta->DataBlocks[j].StructureNameHash)
        {
            case 3461354627:
            {
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(fwEntityDef); i++)
                {
                    fwEntityDef def;
                    std::memcpy(&def, &file.data[meta->DataBlocks[j].DataPointer + i * sizeof(fwEntityDef)], sizeof(fwEntityDef));

                    //	FIX OPENGL
                    def.rotation.w = -def.rotation.w;

                    //	if (def.lodLevel == Unk_1264241711::LODTYPES_DEPTH_ORPHANHD) def.lodDist *= 1.5f;

                    entities.emplace_back(def);
                }
                break;
            }
            case 1860713439:
            {  //	CAR GENERATORS
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(fwEntityDef); i++)
                {
                    CCarGen CarGenerator;
                    memcpy(&CarGenerator, &file.data[meta->DataBlocks[j].DataPointer + i * sizeof(CCarGen)], sizeof(CCarGen));

                    carGenerators.push_back(CarGenerator);
                }
                break;
            }
            case 164374718:  //CMloInstanceDef
            {
                for (int i = 0; i < meta->DataBlocks[j].DataLength / sizeof(CMloInstanceDef); i++)
                {
                    CMloInstanceDef MloInstanceDef;
                    memcpy(&MloInstanceDef, &file.data[meta->DataBlocks[j].DataPointer + i * sizeof(CMloInstanceDef)], sizeof(CMloInstanceDef));

                    CMloInstanceDefs.push_back(MloInstanceDef);
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

/*void YmapPool::RemoveBeta(YmapLoader* ymap, btDynamicsWorld* world)
{
 //ymap->RootObjects.clear();

 /*for (auto &object : *ymap->Objects)
 {
  if (object.rigidBody) {
   world->removeRigidBody(object.rigidBody);
  }
 }

 delete ymap->Objects;
}*/
