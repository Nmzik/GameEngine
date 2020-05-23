#include "YtdLoader.h"

void YtdLoader::finalize(BaseRenderer* _renderer, TextureDictionary* texDictionary, int32_t systemSize)
{
    renderer = _renderer;

    if (texDictionary->Textures.size() != 0)
    {
        for (int i = 0; i < texDictionary->Textures.size(); i++)
        {
            grcTexture* texture = texDictionary->Textures.Get(i);

            if (texture)
            {
                uint8_t* dataPointer = (uint8_t*)((uint64_t)*texture->DataPointer + systemSize);

                //what logic should be there??
                //if we already have loaded texture with the same hash what should we do? Are textures with the same hash different? It seems they are equal
                uint32_t hash = texDictionary->TextureNameHashesPtr.Get(i);

                if (renderer->getTextureManager()->getRefCount(hash) == 0)
                {
                    //we are first who load texture with this hash
                    TextureHandle handle = renderer->createTexture(dataPointer, texture->Width, texture->Height, texture->Levels, texture->Format);
                    renderer->getTextureManager()->addTextureHandle(hash, handle);
                    Texture tex(handle);
                    textures.insert({hash, tex});
                }
                else
                {
                    TextureHandle handle = renderer->getTextureManager()->getTextureHandle(hash);
                    //is it fake texture? object was created with default texture ref
                    if (handle.id == 0)
                    {
						//replace fake texture with the real one
						//downside: old objects won't get updated texture
                        TextureHandle handle = renderer->createTexture(dataPointer, texture->Width, texture->Height, texture->Levels, texture->Format);
                        renderer->getTextureManager()->addTextureHandle(hash, handle, true);
                    }
                    else
                    {
                        textures.insert({hash, Texture(handle)});
                    }
                }
            }
        }
    }
}

YtdLoader::~YtdLoader()
{
    for (auto& tex : textures)
    {
        assert(tex.second.getHandle().id != 0);
        renderer->getTextureManager()->removeTextureHandle(tex.first);
        if (renderer->getTextureManager()->getRefCount(tex.first) == 0)
        {
            renderer->removeTexture(tex.second.getHandle());
        }
    }
}
