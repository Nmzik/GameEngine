#include "YtdLoader.h"

YtdLoader::YtdLoader()
{
}

YtdLoader::YtdLoader(memstream& file, int32_t systemSize)
{
	Init(file, systemSize);
}

void YtdLoader::Init(memstream & file, int32_t systemSize)
{
	enum TextureFormat
	{
		D3DFMT_A8R8G8B8 = 21,
		D3DFMT_A1R5G5B5 = 25,
		D3DFMT_A8 = 28,
		D3DFMT_A8B8G8R8 = 32,
		D3DFMT_L8 = 50,

		// fourCC
		D3DFMT_DXT1 = 0x31545844, //
		D3DFMT_DXT3 = 0x33545844, //
		D3DFMT_DXT5 = 0x35545844, //
		D3DFMT_ATI1 = 0x31495441,
		D3DFMT_ATI2 = 0x32495441,
		D3DFMT_BC7 = 0x20374342,

		//UNKNOWN
	};

	struct {
		uint32_t Unknown_40h;
		uint32_t Unknown_44h; // 0x00000000
		uint32_t Unknown_48h;
		uint32_t Unknown_4Ch; // 0x00000000
		uint16_t Width;
		uint16_t Height;
		uint16_t Unknown_54h; // 0x0001
		uint16_t Stride;
		TextureFormat Format;
		uint8_t Unknown_5Ch; // 0x00
		uint8_t Levels;
		uint16_t Unknown_5Eh; // 0x0000
		uint32_t Unknown_60h; // 0x00000000
		uint32_t Unknown_64h; // 0x00000000
		uint32_t Unknown_68h; // 0x00000000
		uint32_t Unknown_6Ch; // 0x00000000
		uint64_t DataPointer;
		uint32_t Unknown_78h; // 0x00000000
		uint32_t Unknown_7Ch; // 0x00000000
		uint32_t Unknown_80h; // 0x00000000
		uint32_t Unknown_84h; // 0x00000000
		uint32_t Unknown_88h; // 0x00000000
		uint32_t Unknown_8Ch; // 0x00000000
	} Texture;

	ResourceFileBase resourceFileBase;

	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	TextureDictionary texDictionary;

	file.read((char*)&texDictionary, sizeof(TextureDictionary));

	TextureNameHashes.resize(texDictionary.TextureNameHashesPtr.EntriesCount);

	SYSTEM_BASE_PTR(texDictionary.TextureNameHashesPtr.EntriesPointer);

	uint64_t pos = file.tellg();

	file.seekg(texDictionary.TextureNameHashesPtr.EntriesPointer);

	file.read((char*)&TextureNameHashes[0], sizeof(uint32_t) * texDictionary.TextureNameHashesPtr.EntriesCount);

	file.seekg(pos);

	ResourcePointerList64 resourcePointerList;

	file.read((char*)&resourcePointerList, sizeof(ResourcePointerList64));

	SYSTEM_BASE_PTR(resourcePointerList.EntriesPointer);

	file.seekg(resourcePointerList.EntriesPointer);

	for (int i = 0; i < resourcePointerList.EntriesCount; i++)
	{
		uint64_t data_pointer;
		file.read((char*)&data_pointer, sizeof(data_pointer));
		uint64_t posOriginal = file.tellg();

		std::unordered_map<uint32_t, TextureManager::Texture>::iterator it = TextureManager::TexturesMap.find(TextureNameHashes[i]);
		if (it == TextureManager::TexturesMap.end())
		{
			SYSTEM_BASE_PTR(data_pointer);

			file.seekg(data_pointer);

			TextureBase texBase;

			file.read((char*)&texBase, sizeof(TextureBase));
			file.read((char*)&Texture, sizeof(Texture));

			//READ ACTUAL DATA
			GRAPHICS_BASE_PTR(Texture.DataPointer);

			Texture.DataPointer += systemSize;

			int fullLength = 0;
			int length = Texture.Stride * Texture.Height;
			for (int i = 0; i < Texture.Levels; i++)
			{
				fullLength += length;
				length /= 4;
			}

			unsigned int format;
			unsigned int InternalFormat;
			bool compressed = false;

			switch (Texture.Format)
			{
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
					InternalFormat = GL_RGBA8;
					format = GL_UNSIGNED_INT_8_8_8_8_REV;
					break;
				case D3DFMT_A1R5G5B5:
					InternalFormat = GL_RGB5_A1;
					format = GL_UNSIGNED_SHORT_1_5_5_5_REV;
					break;
				case D3DFMT_A8:
					InternalFormat = GL_RGB8;
					format = GL_COMPRESSED_RG_RGTC2;
					break;
				case D3DFMT_A8B8G8R8:
					InternalFormat = GL_RGBA8;
					format = GL_COMPRESSED_RG_RGTC2;
					break;
				case D3DFMT_L8:
					InternalFormat = GL_RG;
					format = GL_COMPRESSED_RG_RGTC2;
					break;
				default:
					printf("UNSUPPORTED FORMAT\n");
					break;
			}

			GLuint textureID = TextureManager::GetTextureID();

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, Texture.Levels - 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture.Levels <= 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 12);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);*/

			if (compressed)
			{
				unsigned int blockSize = (InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || InternalFormat == GL_COMPRESSED_RED_RGTC1) ? 8 : 16;
				unsigned int offset = 0;

				for (unsigned int level = 0; level < Texture.Levels; ++level)
				{
					unsigned int size = ((Texture.Width + 3) / 4)*((Texture.Height + 3) / 4)*blockSize;

					glCompressedTexImage2D(GL_TEXTURE_2D, level, InternalFormat, Texture.Width, Texture.Height, 0, size, &file._buffer.p[Texture.DataPointer] + offset);

					offset += size;
					Texture.Width = std::max(Texture.Width / 2, 1);
					Texture.Height = std::max(Texture.Height / 2, 1);
				}

			}
			else {
				unsigned int offset = 0;

				for (unsigned int level = 0; level < Texture.Levels; ++level)
				{
					unsigned int size = ((Texture.Width + 1) >> 1)  * ((Texture.Height + 1) >> 1) * 4;

					glTexImage2D(GL_TEXTURE_2D, level, InternalFormat, Texture.Width, Texture.Height, 0, GL_BGRA, format, &file._buffer.p[Texture.DataPointer] + offset);

					offset += size;
					Texture.Width /= 2;
					Texture.Height /= 2;
				}
			}


			TextureManager::LoadTexture(TextureNameHashes[i], textureID);
		}
		else {
			it->second.referenceCount++;
		}

		file.seekg(posOriginal);

	}
}


YtdLoader::~YtdLoader()
{
	for (int i = 0; i < TextureNameHashes.size(); i++)
	{
		TextureManager::RemoveTexture(TextureNameHashes[i]);
	}
}
