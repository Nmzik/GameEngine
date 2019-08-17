#include "YddLoader.h"
#include "YdrLoader.h"

void YddLoader::Init(GameRenderer* renderer, memstream& file)
{
    Loaded = true;

    DrawableDictionary* drawableDictionary = (DrawableDictionary*)file.read(sizeof(DrawableDictionary));

    ydrFiles.reserve(drawableDictionary->DrawablesCount1);

    SYSTEM_BASE_PTR(drawableDictionary->HashesPointer);
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

        YdrLoader* ydr = GlobalPool::GetInstance()->ydrPool.create();
        ydr->Init(renderer, file);
        gpuMemory += ydr->gpuMemory;
        ydrFiles.insert({YdrHashes[i], ydr});

        file.seekg(DrawablePointer);
    }
}

YddLoader::~YddLoader()
{
    for (auto& ydr : ydrFiles)
    {
        GlobalPool::GetInstance()->ydrPool.remove(ydr.second);
    }
}
