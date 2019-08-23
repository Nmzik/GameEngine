#pragma once
#include <unordered_map>
#include "YdrLoader.h"

struct DrawableDictionary : ResourceFileBase
{
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    uint32_t Unknown_18h;  // 0x00000001
    uint32_t Unknown_1Ch;  // 0x00000000
    pgPtr<uint32_t> hashes;
    uint32_t Unknown_2Ch;  // 0x00000000
    pgObjectArray<gtaDrawable> drawables;
    uint32_t Unknown_3Ch;  // 0x00000000

    inline void Resolve(memstream& file)
    {
        hashes.Resolve(file);

        drawables.Resolve(file);
    }
};

class YdrLoader;

class YddLoader : public FileType
{
    DrawableDictionary* drawableDictionary;

public:
    std::unordered_map<uint32_t, YdrLoader*> ydrFiles;

    void init(memstream& file) override;
    void finalize(GameRenderer* _renderer, memstream& file) override;
    ~YddLoader();
};
