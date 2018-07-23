#include "YddLoader.h"

void YddLoader::Init(memstream2 & file, int32_t systemSize)
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

		YdrLoader* ydr = YdrPool::getPool().Load();
		ydr->Init(file, systemSize);
		gpuMemory += ydr->gpuMemory;
		YdrFiles.insert({Hashes[i], ydr});

		file.seekg(DrawablePointer);
	}
}

void YddLoader::Remove()
{
	gpuMemory = 0;
	Loaded = false;
	for (auto& ydr : YdrFiles)
	{
		YdrPool::getPool().Remove(ydr.second);
	}
	YdrFiles.clear();
}

YddPool::YddPool()
{
	firstAvailable_ = &ydds[0];

	for (int i = 0; i < 999; i++)
	{
		ydds[i].next = &ydds[i + 1];
	}

	ydds[999].next = NULL;
}

YddPool::~YddPool()
{
}

YddLoader * YddPool::Load()
{
	// Make sure the pool isn't full.
	assert(firstAvailable_ != NULL);

	// Remove it from the available list.
	YddLoader* newYdd = firstAvailable_;
	firstAvailable_ = newYdd->next;

	return newYdd;
}

void YddPool::Remove(YddLoader * ydd)
{
	ydd->Remove();
	ydd->next = firstAvailable_;
	firstAvailable_ = ydd;
}
