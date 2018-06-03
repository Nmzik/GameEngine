#include "YmapLoader.h"

void YmapLoader::Init(memstream& file)
{
	Loaded = true;
	//Could be an additional extraction code here

	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	Meta meta(file);

	for (auto& Block : meta.MetaBlocks)
	{
		if (Block.MetaDataBlock_struct.StructureNameHash == 3545841574)
		{
			std::memcpy(&_CMapData, Block.Data, sizeof(CMapData));

			//Optimization
			Objects.reserve(_CMapData.entities.Count1);
		}

		else if (Block.MetaDataBlock_struct.StructureNameHash == 3461354627)
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CEntityDef); i++)
			{
				CEntityDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CEntityDef)], sizeof(CEntityDef));

				bool isreflproxy = false;
				switch (def.flags)
				{
				case 135790592: //001000000110000000000000000000    prewater proxy (golf course)
				case 135790593: //001000000110000000000000000001    water refl proxy? (mike house)
				case 672661504: //101000000110000000000000000000    vb_ca_prop_tree_reflprox_2
				case 536870912: //100000000000000000000000000000    vb_05_emissive_mirroronly
				case 35127296:  //000010000110000000000000000000    tunnel refl proxy?
				case 39321602:  //000010010110000000000000000010    mlo reflection?
				isreflproxy = true; break;
				}
				if (isreflproxy) {
					continue;
				}

				if (def.lodLevel == Unk_1264241711::LODTYPES_DEPTH_ORPHANHD) def.lodDist *= 1.5f;

				if (def.lodDist <= 0) def.lodDist = 100.0f;
				if (def.childLodDist <= 0) def.childLodDist = 100.0f;

				def.lodDist *= def.lodDist;
				def.childLodDist *= def.childLodDist;

				Objects.emplace_back(def);
			}
		}

		else if (Block.MetaDataBlock_struct.StructureNameHash == 164374718) //CMloInstanceDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CMloInstanceDef); i++)
			{
				CMloInstanceDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CMloInstanceDef)], sizeof(CMloInstanceDef));
				CMloInstanceDefs.push_back(def);
			}
		}

		else if (Block.MetaDataBlock_struct.StructureNameHash == 1860713439) //CCarGen
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CCarGen); i++)
			{
				CCarGen cargen;
				std::memcpy(&cargen, &Block.Data[i * sizeof(CCarGen)], sizeof(CCarGen));
				CCarGens.push_back(cargen);
			}
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

YmapPool::YmapPool()
{
	firstAvailable_ = &ymaps[0];

	for (int i = 0; i < 499; i++)
	{
		ymaps[i].next = &ymaps[i + 1];
	}

	ymaps[499].next = NULL;
}

YmapPool::~YmapPool()
{

}

YmapLoader* YmapPool::Load()
{
	// Make sure the pool isn't full.
	assert(firstAvailable_ != NULL);

	// Remove it from the available list.
	YmapLoader* newYmap = firstAvailable_;
	firstAvailable_ = newYmap->next;

	//newYmap->Init(file);

	return newYmap;
}

void YmapPool::Remove(YmapLoader* ymap)
{
	ymap->Loaded = false;
	ymap->RootObjects.clear();
	ymap->Objects.clear();
	ymap->next = firstAvailable_;
	firstAvailable_ = ymap;
}