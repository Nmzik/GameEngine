#include "YftLoader.h"

YftLoader::YftLoader(memstream& file, glm::vec3 position, glm::quat rotation, glm::vec3 scale, uint32_t hash, btDiscreteDynamicsWorld* world) : Hash(hash)
{
	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	struct {
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
	} FragType;

	file.read((char*)&FragType, sizeof(FragType));

	if ((FragType.DrawablePointer & SYSTEM_BASE) == SYSTEM_BASE) {
		FragType.DrawablePointer = FragType.DrawablePointer & ~0x50000000;
	}
	if ((FragType.DrawablePointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		FragType.DrawablePointer = FragType.DrawablePointer & ~0x60000000;
	}

	file.seekg(FragType.DrawablePointer);

	YdrFile = new YdrLoader(file, position, rotation, scale, hash, world);
}


YftLoader::~YftLoader()
{
	delete YdrFile;
}

glm::mat4 & YftLoader::GetMat4()
{
	return YdrFile->GetMat4();
}

void YftLoader::Draw()
{
	YdrFile->Draw();
}
