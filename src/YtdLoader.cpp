#include "YtdLoader.h"

void YtdLoader::init(memstream& file)
{
    texDictionary = (TextureDictionary*)file.read(sizeof(TextureDictionary));
    texDictionary->Resolve(file);
}

void YtdLoader::finalize(GameRenderer* _renderer, memstream& file)
{
    renderer = _renderer;
    loaded = true;

    if (texDictionary->Textures.size() != 0)
    {
        SYSTEM_BASE_PTR(texDictionary->TextureNameHashesPtr.EntriesPointer);
        file.seekg(texDictionary->TextureNameHashesPtr.EntriesPointer);
        uint32_t* TexturesHashes = (uint32_t*)file.read(sizeof(uint32_t) * texDictionary->TextureNameHashesPtr.EntriesCount);

        for (int i = 0; i < texDictionary->Textures.size(); i++)
        {
            grcTexture* texture = texDictionary->Textures.Get(i);

            texture->DataPointer += file.systemSize;

            int length = texture->Stride * texture->Height;
            for (int i = 0; i < texture->Levels; i++)
            {
                gpuMemory += length;
                length /= 4;
            }

            TextureHandle handle = renderer->createTexture(&file.data[texture->DataPointer], texture->Width, texture->Height, texture->Levels, texture->Format);

            renderer->getTextureManager()->addTexture(TexturesHashes[i], handle);
            Texture tex(handle);
            textures.insert({TexturesHashes[i], tex});
        }
    }
}

YtdLoader::~YtdLoader()
{
    for (auto& tex : textures)
    {
        renderer->getTextureManager()->removeTexture(tex.first);
        renderer->removeTexture(tex.second.getHandle());
    }
}
