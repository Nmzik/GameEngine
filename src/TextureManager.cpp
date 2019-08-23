#include "TextureManager.h"

TextureHandle TextureManager::getTexture(uint32_t hash)
{
    auto iter = textureHandles.find(hash);
    if (iter != textureHandles.end())
    {
        return iter->second.handle;
    }
    else
    {
        printf("Texture with hash %u hasnt been found\n", hash);
        return TextureHandle{0};
    }
}

void TextureManager::addTexture(uint32_t hash, TextureHandle handle)
{
    auto iter = textureHandles.find(hash);
    if (iter != textureHandles.end())
    {
        iter->second.refCount++;
        //printf("Texture with hash %u has already been added\n", hash);
    }
    else
    {
        textureHandles.insert({hash, TextureHandleRef{handle, 1}});
    }
}

void TextureManager::removeTexture(uint32_t hash)
{
    auto iter = textureHandles.find(hash);
    if (iter != textureHandles.end())
    {
        iter->second.refCount--;
        if (iter->second.refCount == 0)
        {
            //printf("Removing texture from TextureCache\n");
            textureHandles.erase(iter);
        }
    }
    else
    {
        printf("Attempting to remove already deleted texture, Abort\n");
    }
}
