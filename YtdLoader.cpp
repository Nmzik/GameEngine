#include "YtdLoader.h"

YtdFile::YtdFile(memstream& file, uint32_t hash) : Hash(hash)
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

	struct {
		uint32_t FileVFT;
		uint32_t FileUnknown;
		uint64_t FilePagesInfoPointer;
	} ResourceFileBase;
	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

	uint64_t SYSTEM_BASE = 0x50000000;
	uint64_t GRAPHICS_BASE = 0x60000000;

	TextureDictionary texDictionary;

	file.read((char*)&texDictionary, sizeof(TextureDictionary));

	std::vector<uint32_t> TextureNameHashes;
	TextureNameHashes.resize(texDictionary.TextureNameHashesPtr.EntriesCount);

	if ((texDictionary.TextureNameHashesPtr.EntriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		texDictionary.TextureNameHashesPtr.EntriesPointer = texDictionary.TextureNameHashesPtr.EntriesPointer & ~0x50000000;
	}
	if ((texDictionary.TextureNameHashesPtr.EntriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		texDictionary.TextureNameHashesPtr.EntriesPointer = texDictionary.TextureNameHashesPtr.EntriesPointer & ~0x60000000;
	}

	uint64_t pos = file.tellg();

	file.seekg(texDictionary.TextureNameHashesPtr.EntriesPointer);

	file.read((char*)&TextureNameHashes[0], sizeof(uint32_t) * texDictionary.TextureNameHashesPtr.EntriesCount);

	file.seekg(pos);

	struct {
		uint64_t EntriesPointer;
		uint16_t EntriesCount;
		uint16_t EntriesCapacity;
	} ResourcePointerList64;

	file.read((char*)&ResourcePointerList64, sizeof(ResourcePointerList64));

	if ((ResourcePointerList64.EntriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		ResourcePointerList64.EntriesPointer = ResourcePointerList64.EntriesPointer & ~0x50000000;
	}
	if ((ResourcePointerList64.EntriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		ResourcePointerList64.EntriesPointer = ResourcePointerList64.EntriesPointer & ~0x60000000;
	}

	file.seekg(ResourcePointerList64.EntriesPointer);

	for (int i = 0; i < ResourcePointerList64.EntriesCount; i++)
	{
		uint64_t data_pointer;
		file.read((char*)&data_pointer, sizeof(data_pointer));
		uint64_t posOriginal = file.tellg();

		if ((data_pointer & SYSTEM_BASE) == SYSTEM_BASE) {
			data_pointer = data_pointer & ~0x50000000;
		}
		if ((data_pointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			data_pointer = data_pointer & ~0x60000000;
		}

		file.seekg(data_pointer);

		TextureBase texBase;

		file.read((char*)&texBase, sizeof(TextureBase));
		file.read((char*)&Texture, sizeof(Texture));

		//READ ACTUAL DATA
		if ((Texture.DataPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			Texture.DataPointer = Texture.DataPointer & ~0x50000000;
		}
		if ((Texture.DataPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			Texture.DataPointer = Texture.DataPointer & ~0x60000000;
		}

		Texture.DataPointer += 8192;

		//file.seekg(Texture.DataPointer);

		int fullLength = 0;
		int length = Texture.Stride * Texture.Height;
		for (int i = 0; i < Texture.Levels; i++)
		{
			fullLength += length;
			length /= 4;
		}

		//uint64_t positbe = file.tellg();
		//printf("%zd\n",positbe);

		/*nv_dds::CDDSImage image;
		image.load("C:\\Users\\nmzik\\Desktop\\cs_rsl_aa_dirttrack3.dds");
		*/

		unsigned int format;
		switch (Texture.Format)
		{
		case D3DFMT_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case D3DFMT_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case D3DFMT_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			printf("UNSUPPORTED FORMAT\n");
			break;
		}

		if (Texture.Format == D3DFMT_DXT1 || Texture.Format == D3DFMT_DXT3 || Texture.Format == D3DFMT_DXT5)
		{
				GLuint textureID;

				glGenTextures(1, &textureID);

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

				unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
				unsigned int offset = 0;

				for (unsigned int level = 0; level < Texture.Levels; ++level)
				{
					unsigned int size = ((Texture.Width + 3) / 4)*((Texture.Height + 3) / 4)*blockSize;

					glCompressedTexImage2D(GL_TEXTURE_2D, level, format, Texture.Width, Texture.Height, 0, size, &file._buffer.p[Texture.DataPointer] + offset);

					offset += size;
					Texture.Width /= 2;
					Texture.Height /= 2;
				}
				TextureManager::LoadTexture(TextureNameHashes[i], textureID);
				//textures.push_back(textureID);
			//}
		}

		file.seekg(posOriginal);

	}
	
}


YtdFile::~YtdFile()
{
}
