#include "YddLoader.h"

YddLoader::YddLoader(memstream& file, glm::vec3 position, glm::quat rotation, glm::vec3 scale, uint32_t hash, btDiscreteDynamicsWorld* world) : Hash(hash)
{
	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	struct {
		uint32_t Unknown_10h; // 0x00000000
		uint32_t Unknown_14h; // 0x00000000
		uint32_t Unknown_18h; // 0x00000001
		uint32_t Unknown_1Ch; // 0x00000000
		uint64_t HashesPointer;
		uint16_t HashesCount1;
		uint16_t HashesCount2;
		uint32_t Unknown_2Ch; // 0x00000000
		uint64_t DrawablesPointer;
		uint16_t DrawablesCount1;
		uint16_t DrawablesCount2;
		uint32_t Unknown_3Ch; // 0x00000000
	} DrawableDictionary;

	file.read((char*)&DrawableDictionary, sizeof(DrawableDictionary));

	YdrFiles.reserve(DrawableDictionary.DrawablesCount1);

	if ((DrawableDictionary.HashesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		DrawableDictionary.HashesPointer = DrawableDictionary.HashesPointer & ~0x50000000;
	}
	if ((DrawableDictionary.HashesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		DrawableDictionary.HashesPointer = DrawableDictionary.HashesPointer & ~0x60000000;
	}

	file.seekg(DrawableDictionary.HashesPointer);

	std::vector<uint32_t> Hashes;
	Hashes.resize(DrawableDictionary.HashesCount1);

	file.read((char*)&Hashes[0], sizeof(uint32_t) * DrawableDictionary.HashesCount1);

	if ((DrawableDictionary.DrawablesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		DrawableDictionary.DrawablesPointer = DrawableDictionary.DrawablesPointer & ~0x50000000;
	}
	if ((DrawableDictionary.DrawablesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		DrawableDictionary.DrawablesPointer = DrawableDictionary.DrawablesPointer & ~0x60000000;
	}

	file.seekg(DrawableDictionary.DrawablesPointer);

	for (int i = 0; i < DrawableDictionary.DrawablesCount1; i++)
	{
		uint64_t DataPointer;
		file.read((char*)&DataPointer, sizeof(uint64_t));

		uint64_t DrawablePointer = file.tellg();

		if ((DataPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			DataPointer = DataPointer & ~0x50000000;
		}
		if ((DataPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			DataPointer = DataPointer & ~0x60000000;
		}

		file.seekg(DataPointer);

		YdrLoader* YdrFile = new YdrLoader(file, position, rotation, scale, Hashes[i], world);
		YdrFiles.push_back(YdrFile);

		file.seekg(DrawablePointer);
	}
}

YddLoader::~YddLoader()
{
	for (std::vector<YdrLoader*>::iterator it = YdrFiles.begin(); it != YdrFiles.end(); /*increment in body*/)
	{
		delete *it;
		it = YdrFiles.erase(it);
	}
}

void YddLoader::Draw()
{
	for (auto& YdrFile : YdrFiles)
	{
		if (YdrFile->isVisible) YdrFile->Draw();
	}
}