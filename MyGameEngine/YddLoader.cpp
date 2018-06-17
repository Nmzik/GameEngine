#include "YddLoader.h"

void YddLoader::Init(memstream2 & file, int32_t systemSize, btDiscreteDynamicsWorld * world)
{
	Loaded = true;

	ResourceFileBase* resourceFileBase = (ResourceFileBase*)file.read(sizeof(ResourceFileBase));

	struct DrawableDictionary {
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
	};

	DrawableDictionary* drawableDictionary = (DrawableDictionary*)file.read(sizeof(DrawableDictionary));

	YdrFiles.reserve(drawableDictionary->DrawablesCount1);

	SYSTEM_BASE_PTR(drawableDictionary->HashesPointer);
	std::vector<uint32_t> Hashes;
	Hashes.resize(drawableDictionary->HashesCount1);
	memcpy(&Hashes[0], &file.data[drawableDictionary->HashesPointer], sizeof(uint32_t) * drawableDictionary->HashesCount1);

	SYSTEM_BASE_PTR(drawableDictionary->DrawablesPointer);
	file.seekg(drawableDictionary->DrawablesPointer);

	//Optimization
	YdrFiles.reserve(drawableDictionary->DrawablesCount1);

	for (int i = 0; i < drawableDictionary->DrawablesCount1; i++)
	{
		uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));

		uint64_t DrawablePointer = file.tellg();

		SYSTEM_BASE_PTR(data_pointer[0]);

		file.seekg(data_pointer[0]);

		YdrLoader* ydr = new YdrLoader();
		ydr->Init(file, systemSize, world, false);
		YdrFiles[Hashes[i]] = ydr;

		file.seekg(DrawablePointer);
	}
}

YddLoader::~YddLoader()
{
	for (auto it = YdrFiles.begin(); it != YdrFiles.end();)
	{
		delete it->second;
		it = YdrFiles.erase(it);
	}
}