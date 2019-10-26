#include "YtdLoader.h"

void YtdLoader::init(memstream& file)
{
    texDictionary = (TextureDictionary*)file.read(sizeof(TextureDictionary));
    texDictionary->Resolve(file);
}

void YtdLoader::finalize(BaseRenderer* _renderer, memstream& file)
{
    renderer = _renderer;
    loaded = true;

    if (texDictionary->Textures.size() != 0)
    {
        for (int i = 0; i < texDictionary->Textures.size(); i++)
        {
            grcTexture* texture = texDictionary->Textures.Get(i);

            texture->DataPointer += file.systemSize;

            TextureHandle handle = renderer->createTexture(&file.data[texture->DataPointer], texture->Width, texture->Height, texture->Levels, texture->Format);

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
        renderer->removeTexture(tex.second.getHandle());
    }
}
