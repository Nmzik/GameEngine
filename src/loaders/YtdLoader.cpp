#include "YtdLoader.h"

void YtdLoader::finalize(BaseRenderer* _renderer, TextureDictionary* texDictionary, int32_t systemSize)
{
    renderer = _renderer;
    
    if (texDictionary->Textures.size() != 0)
    {
        for (int i = 0; i < texDictionary->Textures.size(); i++)
        {
            grcTexture* texture = texDictionary->Textures.Get(i);

            uint8_t* dataPointer = (uint8_t*)((uint64_t)*texture->DataPointer + systemSize);

            TextureHandle handle = renderer->createTexture(dataPointer, texture->Width, texture->Height, texture->Levels, texture->Format);

            uint32_t hash = texDictionary->TextureNameHashesPtr.Get(i);
            renderer->getTextureManager()->addTexture(hash, handle);
            Texture tex(handle);
            textures.insert({hash, tex});
        }
    }
}

YtdLoader::~YtdLoader()
{
    for (auto& tex : textures)
    {
        renderer->getTextureManager()->removeTexture(tex.first);
        if (renderer->getTextureManager()->getRefCount(tex.first) == 0)
			renderer->removeTexture(tex.second.getHandle());
    }
}
