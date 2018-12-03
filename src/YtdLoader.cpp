#include "YtdLoader.h"

void YtdLoader::Init(memstream& file)
{
	Loaded = true;

	TextureDictionary* texDictionary = (TextureDictionary*)file.read(sizeof(TextureDictionary));
	texDictionary->Resolve(file);

	Textures = new std::unordered_map<uint32_t, GLuint>(texDictionary->TextureNameHashesPtr.EntriesCount);

	/*if (texDictionary->Textures.getSize() != 0) {

	 SYSTEM_BASE_PTR(texDictionary->TextureNameHashesPtr.EntriesPointer);
	 file.seekg(texDictionary->TextureNameHashesPtr.EntriesPointer);
	 uint32_t* TexturesHashes = (uint32_t*)file.read(sizeof(uint32_t) * texDictionary->TextureNameHashesPtr.EntriesCount);

	 for (int i = 0; i < texDictionary->Textures.getSize(); i++) {
	   grcTexture* texture = texDictionary->Textures.Get(i);

	   texture->DataPointer += systemSize;

	   unsigned int InternalFormat;
	   unsigned int format;
	   unsigned int type;
	   bool compressed = false;

	   switch (texture->Format) {
	   case D3DFMT_DXT1:
		compressed = true;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	   case D3DFMT_DXT3:
		compressed = true;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	   case D3DFMT_DXT5:
		compressed = true;
		InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	   case D3DFMT_BC7:
		compressed = true;
		InternalFormat = GL_COMPRESSED_RGBA_BPTC_UNORM;
		break;
	   case D3DFMT_ATI1:
		compressed = true;
		InternalFormat = GL_COMPRESSED_RED_RGTC1;
		break;
	   case D3DFMT_ATI2:
		compressed = true;
		InternalFormat = GL_COMPRESSED_RG_RGTC2;
		break;
	   case D3DFMT_A8R8G8B8:
		InternalFormat = GL_RGBA8; // Working
		format = GL_BGRA;
		type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;
	   case D3DFMT_A1R5G5B5: // NOTHING???!!!
		InternalFormat = GL_RGBA;
		format = GL_BGRA;
		type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;
	   case D3DFMT_A8: // COLOR BLACK ALWAYS?
		continue;
		InternalFormat = GL_RED;
		format = GL_RED;
		type = GL_UNSIGNED_BYTE;
		break;
	   case D3DFMT_A8B8G8R8: // CANT Check!
		continue;
		InternalFormat = GL_RGBA;
		format = GL_RGBA;
		type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;
	   case D3DFMT_L8: // FONTS?
		continue;
		InternalFormat = GL_RED;
		format = GL_RED;
		type = GL_UNSIGNED_BYTE;
		break;
	   default:
		printf("UNSUPPORTED FORMAT\n");
		break;
	   }

	   GLuint textureID;
	   glGenTextures(1, &textureID);
	   glBindTexture(GL_TEXTURE_2D, textureID);
	   glTexStorage2D(GL_TEXTURE_2D, texture->Levels, InternalFormat, texture->Width,
			 texture->Height);

	   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, texture->Levels - 1);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		   texture->Levels <= 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
	   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 12);
	   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	   /*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	   //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);*/

	   /*unsigned int offset = 0;

	   if (compressed) {
		unsigned int blockSize = (InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
				InternalFormat == GL_COMPRESSED_RED_RGTC1)
				? 8
				: 16;

		for (unsigned int level = 0; level < texture->Levels; ++level) {
		 unsigned int size =
		  ((texture->Width + 3) / 4) * ((texture->Height + 3) / 4) * blockSize;
		 gpuMemory += size;

		 glCompressedTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, texture->Width,
				 texture->Height, InternalFormat, size,
				 &file.data[texture->DataPointer] + offset);

		 offset += size;
		 texture->Width /= 2;
		 texture->Height /= 2;
		}

	   } else {
		for (unsigned int level = 0; level < texture->Levels; ++level) {
		 unsigned int size =
		  ((texture->Width + 1) >> 1) * ((texture->Height + 1) >> 1) * 4;
		 gpuMemory += size;

		 glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, texture->Width, texture->Height,
			 format, type, &file.data[texture->DataPointer] + offset);

		 offset += size;
		 texture->Width /= 2;
		 texture->Height /= 2;
		}
	   }
	   Textures->insert({ (*TexturesHashes)[i], textureID });
   }
   }*/
}

void YtdLoader::Remove()
{
	if (Textures)
	{
		for (auto& texture : *Textures)
		{
			glDeleteTextures(1, &texture.second);
		}
		delete Textures;
		Textures = nullptr;
	}
}
