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

    TextureHandle getTexture(uint32_t hash);
    void addTexture(uint32_t hash, TextureHandle handle);
    void removeTexture(uint32_t hash);
};
