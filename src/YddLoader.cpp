#include "YddLoader.h"
#include "YdrLoader.h"

void YddLoader::Init(memstream2 & file, int32_t systemSize)
{
	Loaded = true;

	DrawableDictionary* drawableDictionary = (DrawableDictionary*)file.read(sizeof(DrawableDictionary));

	YdrFiles = new std::unordered_map<uint32_t, YdrLoader*>();
	YdrFiles->reserve(drawableDictionary->DrawablesCount1);

	SYSTEM_BASE_PTR(drawableDictionary->HashesPointer);
	std::vector<uint32_t> Hashes;
	Hashes.resize(drawableDictionary->HashesCount1);
	memcpy(&Hashes[0], &file.data[drawableDictionary->HashesPointer], sizeof(uint32_t) * drawableDictionary->HashesCount1);

	SYSTEM_BASE_PTR(drawableDictionary->DrawablesPointer);
	file.seekg(drawableDictionary->DrawablesPointer);

	for (int i = 0; i < drawableDictionary->DrawablesCount1; i++)
	{
		uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));

		uint64_t DrawablePointer = file.tellg();

		SYSTEM_BASE_PTR(data_pointer[0]);

		file.seekg(data_pointer[0]);

		YdrLoader* ydr = YdrPool.getPool().Load();
		ydr->Init(file, systemSize);
		gpuMemory += ydr->gpuMemory;
		YdrFiles->insert({Hashes[i], ydr});

		file.seekg(DrawablePointer);
	}
}

void YddLoader::Remove()
{
	gpuMemory = 0;

	for (auto& ydr : *YdrFiles)
	{
		YdrPool.getPool().Remove(ydr.second);
	}
	delete YdrFiles;
}