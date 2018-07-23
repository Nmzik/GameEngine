#include "YftLoader.h"

void YftLoader::Init(memstream2 & file, int32_t systemSize)
{
	Loaded = true;

	ResourceFileBase* resourceFileBase = (ResourceFileBase*)file.read(sizeof(ResourceFileBase));

	struct FragType {
		uint32_t Unknown_10h; // 0x00000000
		uint32_t Unknown_14h; // 0x00000000
		uint32_t Unknown_18h; // 0x00000000
		uint32_t Unknown_1Ch; // 0x00000000
		uint32_t Unknown_20h;
		uint32_t Unknown_24h;
		uint32_t Unknown_28h;
		uint32_t Unknown_2Ch;
		uint64_t DrawablePointer;
		uint64_t Unknown_28h_Pointer;
		uint64_t Unknown_30h_Pointer;
		uint32_t Count0;
		uint32_t Unknown_4Ch;
		uint32_t Unknown_50h; // 0x00000000
		uint32_t Unknown_54h; // 0x00000000
		uint64_t NamePointer;
		ResourcePointerList64 Clothes;
		uint32_t Unknown_70h; // 0x00000000
		uint32_t Unknown_74h; // 0x00000000
		uint32_t Unknown_78h; // 0x00000000
		uint32_t Unknown_7Ch; // 0x00000000
		uint32_t Unknown_80h; // 0x00000000
		uint32_t Unknown_84h; // 0x00000000
		uint32_t Unknown_88h; // 0x00000000
		uint32_t Unknown_8Ch; // 0x00000000
		uint32_t Unknown_90h; // 0x00000000
		uint32_t Unknown_94h; // 0x00000000
		uint32_t Unknown_98h; // 0x00000000
		uint32_t Unknown_9Ch; // 0x00000000
		uint32_t Unknown_A0h; // 0x00000000
		uint32_t Unknown_A4h; // 0x00000000
		uint64_t Unknown_A8h_Pointer;
		uint32_t Unknown_B0h; // 0x00000000
		uint32_t Unknown_B4h; // 0x00000000
		uint32_t Unknown_B8h;
		uint32_t Unknown_BCh;
		uint32_t Unknown_C0h;
		uint32_t Unknown_C4h;
		uint32_t Unknown_C8h;
		uint32_t Unknown_CCh;
		uint32_t Unknown_D0h;
		uint32_t Unknown_D4h;
		uint8_t Unknown_D8h;
		uint8_t Count3;
		uint16_t Unknown_DAh;
		uint32_t Unknown_DCh; // 0x00000000
		uint64_t Unknown_E0h_Pointer;
		uint32_t Unknown_E8h; // 0x00000000
		uint32_t Unknown_ECh; // 0x00000000
		uint64_t PhysicsLODGroupPointer;
		uint64_t Unknown_F8h_Pointer;
		uint32_t Unknown_100h; // 0x00000000
		uint32_t Unknown_104h; // 0x00000000
		uint32_t Unknown_108h; // 0x00000000
		uint32_t Unknown_10Ch; // 0x00000000
		ResourceSimpleList64Ptr LightAttributesPtr;
		uint64_t Unknown_120h_Pointer;
		uint32_t Unknown_128h; // 0x00000000
		uint32_t Unknown_12Ch; // 0x00000000
	};

	FragType* fragType = (FragType*)file.read(sizeof(FragType));

	SYSTEM_BASE_PTR(fragType->DrawablePointer);

	file.seekg(fragType->DrawablePointer);

	YdrFile = YdrPool::getPool().Load();
	YdrFile->isYft = true;
	YdrFile->Init(file, systemSize);
	gpuMemory += YdrFile->gpuMemory;

	struct FragPhysicsLODGroup {
		uint32_t VFT;
		uint32_t Unknown_04h; // 0x00000001
		uint32_t Unknown_08h; // 0x00000000
		uint32_t Unknown_0Ch; // 0x00000000
		uint64_t PhysicsLOD1Pointer;
		uint64_t PhysicsLOD2Pointer;
		uint64_t PhysicsLOD3Pointer;
		uint32_t Unknown_28h; // 0x00000000
		uint32_t Unknown_2Ch; // 0x00000000
	};

	struct FragPhysicsLOD {
		uint32_t VFT;
		uint32_t Unknown_04h; // 0x00000001
		uint32_t Unknown_08h; // 0x00000000
		uint32_t Unknown_0Ch; // 0x00000000
		uint32_t Unknown_10h; // 0x00000000
		uint32_t Unknown_14h;
		uint32_t Unknown_18h;
		uint32_t Unknown_1Ch;
		uint64_t ArticulatedBodyTypePointer;
		uint64_t Unknown_28h_Pointer;
		glm::vec4 Unknown_30h;
		glm::vec4 Unknown_40h;
		glm::vec4 Unknown_50h;
		glm::vec4 Unknown_60h;
		glm::vec4 Unknown_70h;
		glm::vec4 Unknown_80h;
		glm::vec4 Unknown_90h;
		glm::vec4 Unknown_A0h;
		glm::vec4 Unknown_B0h;
		uint64_t GroupNamesPointer;
		uint64_t GroupsPointer;
		uint64_t ChildrenPointer;
		uint64_t Archetype1Pointer;
		uint64_t Archetype2Pointer;
		uint64_t BoundPointer;
		uint64_t InertiaTensorsPointer;
		uint64_t Unknown_F8h_Pointer;
		uint64_t FragTransformsPointer;
		uint64_t Unknown_108h_Pointer;
		uint64_t Unknown_110h_Pointer;
		uint8_t Count1;
		uint8_t Count2;
		uint8_t GroupsCount;
		uint8_t Unknown_11Bh;
		uint8_t Unknown_11Ch;
		uint8_t ChildrenCount;
		uint8_t Count3;
		uint8_t Unknown_11Fh; // 0x00
		uint32_t Unknown_120h; // 0x00000000
		uint32_t Unknown_124h; // 0x00000000
		uint32_t Unknown_128h; // 0x00000000
		uint32_t Unknown_12Ch; // 0x00000000
	};

	struct FragPhysTypeChild {
		uint32_t VFT;
		uint32_t Unknown_04h; // 0x00000001
		float Unknown_08h;
		float Unknown_0Ch;
		float Unknown_10h;
		uint32_t Unknown_14h; // 0x00000000
		uint32_t Unknown_18h; // 0x00000000
		uint32_t Unknown_1Ch; // 0x00000000
		uint32_t Unknown_20h; // 0x00000000
		uint32_t Unknown_24h; // 0x00000000
		uint32_t Unknown_28h; // 0x00000000
		uint32_t Unknown_2Ch; // 0x00000000
		uint32_t Unknown_30h; // 0x00000000
		uint32_t Unknown_34h; // 0x00000000
		uint32_t Unknown_38h; // 0x00000000
		uint32_t Unknown_3Ch; // 0x00000000
		uint32_t Unknown_40h; // 0x00000000
		uint32_t Unknown_44h; // 0x00000000
		uint32_t Unknown_48h; // 0x00000000
		uint32_t Unknown_4Ch; // 0x00000000
		uint32_t Unknown_50h; // 0x00000000
		uint32_t Unknown_54h; // 0x00000000
		uint32_t Unknown_58h; // 0x00000000
		uint32_t Unknown_5Ch; // 0x00000000
		uint32_t Unknown_60h; // 0x00000000
		uint32_t Unknown_64h; // 0x00000000
		uint32_t Unknown_68h; // 0x00000000
		uint32_t Unknown_6Ch; // 0x00000000
		uint32_t Unknown_70h; // 0x00000000
		uint32_t Unknown_74h; // 0x00000000
		uint32_t Unknown_78h; // 0x00000000
		uint32_t Unknown_7Ch; // 0x00000000
		uint32_t Unknown_80h; // 0x00000000
		uint32_t Unknown_84h; // 0x00000000
		uint32_t Unknown_88h; // 0x00000000
		uint32_t Unknown_8Ch; // 0x00000000
		uint32_t Unknown_90h; // 0x00000000
		uint32_t Unknown_94h; // 0x00000000
		uint32_t Unknown_98h; // 0x00000000
		uint32_t Unknown_9Ch; // 0x00000000
		uint64_t Drawable1Pointer;
		uint64_t Drawable2Pointer;
		uint64_t EvtSetPointer;
		uint32_t Unknown_B8h; // 0x00000000
		uint32_t Unknown_BCh; // 0x00000000
		uint32_t Unknown_C0h; // 0x00000000
		uint32_t Unknown_C4h; // 0x00000000
		uint32_t Unknown_C8h; // 0x00000000
		uint32_t Unknown_CCh; // 0x00000000
		uint32_t Unknown_D0h; // 0x00000000
		uint32_t Unknown_D4h; // 0x00000000
		uint32_t Unknown_D8h; // 0x00000000
		uint32_t Unknown_DCh; // 0x00000000
		uint32_t Unknown_E0h; // 0x00000000
		uint32_t Unknown_E4h; // 0x00000000
		uint32_t Unknown_E8h; // 0x00000000
		uint32_t Unknown_ECh; // 0x00000000
		uint32_t Unknown_F0h; // 0x00000000
		uint32_t Unknown_F4h; // 0x00000000
		uint32_t Unknown_F8h; // 0x00000000
		uint32_t Unknown_FCh; // 0x00000000
	};

	/*SYSTEM_BASE_PTR(FragType.PhysicsLODGroupPointer);

	file.seekg(FragType.PhysicsLODGroupPointer);

	file.read((char*)&FragPhysicsLODGroup, sizeof(FragPhysicsLODGroup));

	SYSTEM_BASE_PTR(FragPhysicsLODGroup.PhysicsLOD1Pointer);

	file.seekg(FragPhysicsLODGroup.PhysicsLOD1Pointer);

	file.read((char*)&FragPhysicsLOD, sizeof(FragPhysicsLOD));

	SYSTEM_BASE_PTR(FragPhysicsLOD.ChildrenPointer);

	file.seekg(FragPhysicsLOD.ChildrenPointer);
	/*if (need) {

	for (int i = 0; i < FragPhysicsLOD.ChildrenCount; i++)
	{
	uint64_t DataPointer;
	file.read((char*)&DataPointer, sizeof(uint64_t));

	uint64_t ChildrenPointer = file.tellg();

	SYSTEM_BASE_PTR(DataPointer);

	file.seekg(DataPointer);

	file.read((char*)&FragPhysTypeChild, sizeof(FragPhysTypeChild));

	SYSTEM_BASE_PTR(FragPhysTypeChild.Drawable1Pointer);

	file.seekg(FragPhysTypeChild.Drawable1Pointer);

	//YdrLoader* ydr = new YdrLoader(file, systemSize, world, true);

	/*if (ydr->.size() != 0) {
	wheels.push_back(ydr);
	}
	else {
	delete ydr;
	}

	file.seekg(ChildrenPointer);
	}
	}*/

	//SYSTEM_BASE_PTR(FragPhysicsLOD.BoundPointer);

	//YbnLoader* loader = new YbnLoader(world, file, hash);*/
}


void YftLoader::Remove()
{
	gpuMemory = 0;
	Loaded = false;
	if (YdrFile) {
		YdrPool::getPool().Remove(YdrFile);
		YdrFile = nullptr;
	}
}

YftPool::YftPool()
{
	firstAvailable_ = &yfts[0];

	for (int i = 0; i < 999; i++)
	{
		yfts[i].next = &yfts[i + 1];
	}

	yfts[999].next = NULL;
}

YftPool::~YftPool()
{
}

YftLoader * YftPool::Load()
{
	// Make sure the pool isn't full.
	assert(firstAvailable_ != NULL);

	// Remove it from the available list.
	YftLoader* newYft = firstAvailable_;
	firstAvailable_ = newYft->next;

	return newYft;
}

void YftPool::Remove(YftLoader * yft)
{
	yft->Remove();
	yft->next = firstAvailable_;
	firstAvailable_ = yft;
}
