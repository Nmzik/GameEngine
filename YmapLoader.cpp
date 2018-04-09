#include "YmapLoader.h"

#define SYSTEM_BASE 0x50000000
#define GRAPHICS_BASE 0x60000000

YmapLoader::YmapLoader(memstream& file)
{
/*	//ADD NEED
	uint32_t rsc7;
	file.read((char*)&rsc7, sizeof(uint32_t));

	if (rsc7 == 0x37435352) {
		printf("NOT IMPLEMENTED!");
	}
	else {
		printf("HERE");
	}*/

	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	Meta meta(file);

	for (auto& Block : meta.MetaBlocks)
	{
		if (Block.MetaDataBlock_struct.StructureNameHash == 3545841574)
		{
			std::memcpy(&_CMapData, Block.Data, sizeof(CMapData));
		}

		//Optimization
		CEntityDefs.reserve(_CMapData.entities.Count1);
		ModelMatrices.reserve(_CMapData.entities.Count1);

		if (Block.MetaDataBlock_struct.StructureNameHash == 3461354627)
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

				CEntityDefs.push_back(def);
				ModelMatrices.emplace_back(glm::translate(glm::mat4(), def.position) * glm::toMat4(glm::quat(-def.rotation.w, def.rotation.x, def.rotation.y, def.rotation.z)) * glm::scale(glm::mat4(), glm::vec3(def.scaleXY, def.scaleXY, def.scaleZ)));
			}
		}

		if (Block.MetaDataBlock_struct.StructureNameHash == 164374718) //CMloInstanceDef
		{
			for (int i = 0; i < Block.MetaDataBlock_struct.DataLength / sizeof(CMloInstanceDef); i++)
			{
				CMloInstanceDef def;
				std::memcpy(&def, &Block.Data[i * sizeof(CMloInstanceDef)], sizeof(CMloInstanceDef));
				CMloInstanceDefs.push_back(def);
			}
		}
	}
}


YmapLoader::~YmapLoader()
{

}
