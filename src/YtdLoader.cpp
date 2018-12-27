#include "YtdLoader.h"

#include "Texture.h"

void YtdLoader::Init(memstream& file)
{
	Loaded = true;

	TextureDictionary* texDictionary = (TextureDictionary*)file.read(sizeof(TextureDictionary));
	texDictionary->Resolve(file);

	if (texDictionary->Textures.getSize() != 0) {

		SYSTEM_BASE_PTR(texDictionary->TextureNameHashesPtr.EntriesPointer);
		file.seekg(texDictionary->TextureNameHashesPtr.EntriesPointer);
		uint32_t* TexturesHashes = (uint32_t*)file.read(sizeof(uint32_t) * texDictionary->TextureNameHashesPtr.EntriesCount);

		for (int i = 0; i < texDictionary->Textures.getSize(); i++) {
			grcTexture* texture = texDictionary->Textures.Get(i);

			texture->DataPointer += file.systemSize;

			int length = texture->Stride * texture->Height;
			for (int i = 0; i < texture->Levels; i++)
			{
				gpuMemory += length;
				length /= 4;
			}

			Textures.emplace(std::piecewise_construct, std::forward_as_tuple(TexturesHashes[i]), std::forward_as_tuple(TexturesHashes[i], texture, file));
		}
	}
}
