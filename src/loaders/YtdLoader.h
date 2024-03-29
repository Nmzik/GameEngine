#pragma once
#include <unordered_map>

#include "FileType.h"

#include "../Texture.h"

struct TextureBase : datBase
{
    uint32_t Unknown_8h;   // 0x00000000
    uint32_t Unknown_Ch;   // 0x00000000
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    uint32_t Unknown_18h;  // 0x00000000
    uint32_t Unknown_1Ch;  // 0x00000000
    uint32_t Unknown_20h;  // 0x00000000
    uint32_t Unknown_24h;  // 0x00000000
    uint64_t NamePointer;
    uint32_t Unknown_30h;
    uint32_t Unknown_34h;  // 0x00000000
    uint32_t Unknown_38h;  // 0x00000000
    uint32_t Unknown_3Ch;  // 0x00000000
};

struct grcTexture : TextureBase
{
    uint32_t Unknown_40h;
    uint32_t Unknown_44h;  // 0x00000000
    uint32_t Unknown_48h;
    uint32_t Unknown_4Ch;  // 0x00000000
    uint16_t Width;
    uint16_t Height;
    uint16_t Unknown_54h;  // 0x0001
    uint16_t Stride;
    TextureFormat Format;
    uint8_t Unknown_5Ch;  // 0x00
    uint8_t Levels;
    uint16_t Unknown_5Eh;  // 0x0000
    uint32_t Unknown_60h;  // 0x00000000
    uint32_t Unknown_64h;  // 0x00000000
    uint32_t Unknown_68h;  // 0x00000000
    uint32_t Unknown_6Ch;  // 0x00000000
    pgPtr<uint8_t> DataPointer;
    uint32_t Unknown_78h;  // 0x00000000
    uint32_t Unknown_7Ch;  // 0x00000000
    uint32_t Unknown_80h;  // 0x00000000
    uint32_t Unknown_84h;  // 0x00000000
    uint32_t Unknown_88h;  // 0x00000000
    uint32_t Unknown_8Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        DataPointer.ResolveGraphicsPointer(file);
    }
};

struct TextureDictionary : pgBase
{
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    uint32_t Unknown_18h;  // 0x00000001
    uint32_t Unknown_1Ch;  // 0x00000000
    pgArray<uint32_t> TextureNameHashesPtr;
    pgObjectArray<grcTexture> Textures;

    void Resolve(memstream& file)
    {
        TextureNameHashesPtr.Resolve(file);
        Textures.Resolve(file);
    }
};

class YtdLoader : public FileType
{
    BaseRenderer* renderer = nullptr;

public:
    std::unordered_map<uint32_t, Texture> textures;

    YtdLoader* dependency;

    void finalize(BaseRenderer* _renderer, TextureDictionary* texDictionary, int32_t systemSize);

    YtdLoader() = default;
    ~YtdLoader();
};
