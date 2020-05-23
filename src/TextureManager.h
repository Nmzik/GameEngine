#pragma once
#include <unordered_map>

#include "Texture.h"

struct TextureHandleRef
{
    TextureHandle handle;
    uint32_t refCount;
};

class TextureManager
{
    std::unordered_map<uint32_t, TextureHandleRef> textureHandles;

public:
    TextureManager() = default;
    ~TextureManager() = default;

    int getRefCount(uint32_t hash);
    void addTextureHandle(uint32_t hash, TextureHandle handle, bool replaceFake = false);
    TextureHandle getTextureHandle(uint32_t hash);
    void removeTextureHandle(uint32_t hash);
};
