#pragma once

#define OPENGL_NAME_CACHE_SIZE 1024

#include <iostream>
#include <unordered_map>
#include <vector>

class TextureManager
{
public:
    struct Texture
    {
        GLuint TextureID;
        uint32_t referenceCount;
    };

    static TextureManager& GetTextureManager()
    {
        return texManager;
    }

    GLuint DefaultTexture;

    std::unordered_map<uint32_t, Texture> TexturesMap;
    GLuint GetTexture(uint32_t textureHash);
    void LoadTexture(uint32_t Hash, GLuint TextureID);
    void RemoveTexture(uint32_t Hash);

    static TextureManager texManager;
};
