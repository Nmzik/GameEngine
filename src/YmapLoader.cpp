#include "YmapLoader.h"
#include <btBulletDynamicsCommon.h>
#include "Object.h"

void YmapLoader::Init(memstream& file)
{
	//	Could be an additional extraction code here

	Meta meta(file);

	Objects = new std::vector<Object>();

	for (auto& Block : meta.MetaBlocks)
	{
		switch (Block.MetaDataBlock_struct->StructureNameHash)
		{
			case 3545841574:
			{
				std::memcpy(&_CMapData, &file.data[Block.MetaDataBlock_struct->DataPointer], sizeof(CMapData));

				//	Optimization
				Objects->reserve(_CMapData.entities.Count1);

				break;
			}
			case 3461354627:
			{
				for (int i = 0; i < Block.MetaDataBlock_struct->DataLength / sizeof(fwEntityDef); i++)
				{
					fwEntityDef def;
					std::memcpy(&def, &file.data[Block.MetaDataBlock_struct->DataPointer + i * sizeof(fwEntityDef)], sizeof(fwEntityDef));

					bool isreflproxy = false;
					switch (def.flags)
					{
						case 135790592: //	001000000110000000000000000000    prewater proxy (golf course)
						case 135790593: //	001000000110000000000000000001    water refl proxy? (mike house)
						case 672661504: //	101000000110000000000000000000    vb_ca_prop_tree_reflprox_2
						case 536870912: //	100000000000000000000000000000    vb_05_emissive_mirroronly
						case 35127296:  //	000010000110000000000000000000    tunnel refl proxy?
						case 39321602:  //	000010010110000000000000000010    mlo reflection?
							isreflproxy = true;
							break;
					}
					if (isreflproxy)
					{
						continue;
					}

					//	FIX OPENGL
					def.rotation.w = -def.rotation.w;

					//	if (def.lodLevel == Unk_1264241711::LODTYPES_DEPTH_ORPHANHD) def.lodDist *= 1.5f;

					Objects->emplace_back(def);
				}
				break;
			}
			default: break;
		}

		/*else if (Block.MetaDataBlock_struct->StructureNameHash == 164374718) //CMloInstanceDef
		{
		 size_t curLength = CMloInstanceDefs.size();
		 CMloInstanceDefs.resize(curLength + Block.MetaDataBlock_struct->DataLength / sizeof(CMloInstanceDef));
		 std::memcpy(&CMloInstanceDefs[curLength], &file.data[Block.MetaDataBlock_struct->DataPointer], Block.MetaDataBlock_struct->DataLength);
		}

		else if (Block.MetaDataBlock_struct->StructureNameHash == 1860713439) //CCarGen
		{
		 size_t curLength = CCarGens.size();
		 CCarGens.resize(curLength + Block.MetaDataBlock_struct->DataLength / sizeof(CCarGen));
		 std::memcpy(&CCarGens[curLength], &file.data[Block.MetaDataBlock_struct->DataPointer], Block.MetaDataBlock_struct->DataLength);
		}*/
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

void YmapLoader::Remove()
{
	delete Objects;
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
