#include "YddLoader.h"

YddLoader::YddLoader() : Loaded(false)
{
}

void YddLoader::Init(memstream & file, int32_t systemSize, btDiscreteDynamicsWorld * world)
{
	Loaded = true;

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

	TranslatePTR(DrawableDictionary.HashesPointer);

	file.seekg(DrawableDictionary.HashesPointer);

	std::vector<uint32_t> Hashes;
	Hashes.resize(DrawableDictionary.HashesCount1);

	file.read((char*)&Hashes[0], sizeof(uint32_t) * DrawableDictionary.HashesCount1);

	TranslatePTR(DrawableDictionary.DrawablesPointer);

	file.seekg(DrawableDictionary.DrawablesPointer);

	//Optimization
	YdrFiles.reserve(DrawableDictionary.DrawablesCount1);

	for (int i = 0; i < DrawableDictionary.DrawablesCount1; i++)
	{
		uint64_t DataPointer;
		file.read((char*)&DataPointer, sizeof(uint64_t));

		uint64_t DrawablePointer = file.tellg();

		TranslatePTR(DataPointer);

		file.seekg(DataPointer);

		YdrLoader* YdrFile = new YdrLoader(file, systemSize, world, false);
		YdrFiles[Hashes[i]] = YdrFile;

		file.seekg(DrawablePointer);
	}
}

YddLoader::YddLoader(memstream& file, int32_t systemSize, btDiscreteDynamicsWorld* world)
{
	Init(file, systemSize, world);
}

YddLoader::~YddLoader()
{
	for (auto it = YdrFiles.begin(); it != YdrFiles.end();)
	{
		delete it->second;
		it = YdrFiles.erase(it);
	}
}