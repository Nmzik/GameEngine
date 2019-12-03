#pragma once
#include <cstdint>

enum TextureFormat
{
    D3DFMT_A8R8G8B8 = 21,
    D3DFMT_A1R5G5B5 = 25,
    D3DFMT_A8 = 28,
    D3DFMT_A8B8G8R8 = 32,
    D3DFMT_L8 = 50,

    // fourCC
    D3DFMT_DXT1 = 0x31545844,  //
    D3DFMT_DXT3 = 0x33545844,  //
    D3DFMT_DXT5 = 0x35545844,  //
    D3DFMT_ATI1 = 0x31495441,
    D3DFMT_ATI2 = 0x32495441,
    D3DFMT_BC7 = 0x20374342,

    //    UNKNOWN
};

struct TextureHandle
{
    int32_t id;
};

class Texture
{
    TextureHandle handle;

public:
    Texture() = default;
    Texture(TextureHandle texHandle);
    ~Texture();

    TextureHandle getHandle()
    {
        return handle;
    }
};
