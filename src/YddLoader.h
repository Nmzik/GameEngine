#pragma once
#include <unordered_map>
#include "FileType.h"

struct DrawableDictionary : ResourceFileBase
{
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    uint32_t Unknown_18h;  // 0x00000001
    uint32_t Unknown_1Ch;  // 0x00000000
    uint64_t HashesPointer;
    uint16_t HashesCount1;
    uint16_t HashesCount2;
    uint32_t Unknown_2Ch;  // 0x00000000
    uint64_t DrawablesPointer;
    uint16_t DrawablesCount1;
    uint16_t DrawablesCount2;
    uint32_t Unknown_3Ch;  // 0x00000000
};

class YdrLoader;

class YddLoader : public FileType
{
public:
    std::unordered_map<uint32_t, YdrLoader*> ydrFiles;

    void Init(GameRenderer* renderer, memstream& file) override;
    ~YddLoader();
};
