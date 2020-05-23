#include "TextureManager.h"

#include <cassert>

int TextureManager::getRefCount(uint32_t hash)
{
    auto iter = textureHandles.find(hash);
    if (iter != textureHandles.end())
    {
        return iter->second.refCount;
    }
    else
    {
        return 0;
    }
}

TextureHandle TextureManager::getTextureHandle(uint32_t hash)
{
    auto iter = textureHandles.find(hash);
    if (iter != textureHandles.end())
    {
        iter->second.refCount++;
        return iter->second.handle;
        //printf("Texture with hash %u has already been added\n", hash);
    }
    else
    {
        textureHandles.insert({hash, TextureHandleRef{TextureHandle{0}, 1}});
        return TextureHandle{0};
    }
}

void TextureManager::addTextureHandle(uint32_t hash, TextureHandle handle, bool replaceFake)
{
    if (replaceFake)
    {
        auto iter = textureHandles.find(hash);
        if (iter != textureHandles.end())
        {
            iter->second.handle = handle;
        }
        else
        {
            assert(false);
        }
	} else
		textureHandles.insert({hash, TextureHandleRef{handle, 1}});
}

void TextureManager::removeTextureHandle(uint32_t hash)
{
    auto iter = textureHandles.find(hash);
    if (iter != textureHandles.end())
    {
        assert(iter->second.refCount > 0);
        iter->second.refCount--;
        if (iter->second.refCount == 0)
        {
            //printf("Removing texture from TextureCache\n");
            textureHandles.erase(iter);
        }
    }
    else
    {
        assert(false);
        printf("Attempting to remove already deleted texture, Abort\n");
    }
}
