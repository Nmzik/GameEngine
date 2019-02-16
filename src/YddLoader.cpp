#include "YddLoader.h"
#include "YdrLoader.h"

void YddLoader::Init(memstream& file)
{
	Loaded = true;

	DrawableDictionary* drawableDictionary = (DrawableDictionary*)file.read(sizeof(DrawableDictionary));
	drawableDictionary->Resolve(file);

	ydrFiles.reserve(drawableDictionary->Drawables.getSize());

	/*for (int i = 0; i < drawableDictionary->Drawables.getSize(); i++)
	{
		std::unique_ptr<YdrLoader> ydr = std::make_unique<YdrLoader>();

		ydr->Init(file);
		gpuMemory += ydr->gpuMemory;
		ydrFiles.insert({drawableDictionary->HashesPointer.Get(i), std::move(ydr)});
	}*/
	/*SYSTEM_BASE_PTR(drawableDictionary->HashesPointer);
	file.seekg(drawableDictionary->HashesPointer);
	uint32_t* YdrHashes = (uint32_t*)file.read(sizeof(uint32_t) * drawableDictionary->HashesCount1);

	SYSTEM_BASE_PTR(drawableDictionary->DrawablesPointer);
	file.seekg(drawableDictionary->DrawablesPointer);

	for (int i = 0; i < drawableDictionary->DrawablesCount1; i++)
	{
		uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));

		uint64_t DrawablePointer = file.tellg();

		SYSTEM_BASE_PTR(data_pointer[0]);

		file.seekg(data_pointer[0]);

		std::unique_ptr<YdrLoader> ydr = std::make_unique<YdrLoader>();
		ydr->Init(file);
		gpuMemory += ydr->gpuMemory;
		ydrFiles.insert({YdrHashes[i], std::move(ydr)});

		file.seekg(DrawablePointer);
	}*/
}

YddLoader::~YddLoader()
{
}