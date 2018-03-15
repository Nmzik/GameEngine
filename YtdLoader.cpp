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

	ResourceFileBase resourceFileBase;

	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	TextureDictionary texDictionary;

	file.read((char*)&texDictionary, sizeof(TextureDictionary));

	TextureNameHashes.resize(texDictionary.TextureNameHashesPtr.EntriesCount);

	TranslatePTR(texDictionary.TextureNameHashesPtr.EntriesPointer);

	uint64_t pos = file.tellg();

	file.seekg(texDictionary.TextureNameHashesPtr.EntriesPointer);

	file.read((char*)&TextureNameHashes[0], sizeof(uint32_t) * texDictionary.TextureNameHashesPtr.EntriesCount);

	file.seekg(pos);

	ResourcePointerList64 resourcePointerList;

	file.read((char*)&resourcePointerList, sizeof(ResourcePointerList64));

	TranslatePTR(resourcePointerList.EntriesPointer);

	file.seekg(resourcePointerList.EntriesPointer);

	for (int i = 0; i < resourcePointerList.EntriesCount; i++)
	{
		uint64_t data_pointer;
		file.read((char*)&data_pointer, sizeof(data_pointer));
		uint64_t posOriginal = file.tellg();

		TranslatePTR(data_pointer);

		file.seekg(data_pointer);

		TextureBase texBase;

		file.read((char*)&texBase, sizeof(TextureBase));
		file.read((char*)&Texture, sizeof(Texture));

		//READ ACTUAL DATA
		TranslatePTR(Texture.DataPointer);

		Texture.DataPointer += 8192;

		int fullLength = 0;
		int length = Texture.Stride * Texture.Height;
		for (int i = 0; i < Texture.Levels; i++)
		{
			fullLength += length;
			length /= 4;
		}

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
		case D3DFMT_BC7:
			format = GL_COMPRESSED_RGBA_BPTC_UNORM;
		case D3DFMT_ATI1:
			format = GL_COMPRESSED_RED_RGTC1;
		case D3DFMT_ATI2:
			format = GL_COMPRESSED_RG_RGTC2;
		case D3DFMT_A8R8G8B8:
			format = GL_UNSIGNED_INT_8_8_8_8_REV;
		case D3DFMT_A1R5G5B5:
			format = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		case D3DFMT_A8:
			format = GL_COMPRESSED_RED_GREEN_RGTC2_EXT;
		case D3DFMT_A8B8G8R8:
			format = GL_COMPRESSED_RED_GREEN_RGTC2_EXT;
		case D3DFMT_L8:
			format = GL_COMPRESSED_RED_GREEN_RGTC2_EXT;
		default:
			printf("UNSUPPORTED FORMAT\n");
			break;
		}

		GLuint textureID;

		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, Texture.Levels - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture.Levels <= 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);*/

		if (Texture.Format == D3DFMT_DXT1 || Texture.Format == D3DFMT_DXT3 || Texture.Format == D3DFMT_DXT5 || Texture.Format == D3DFMT_BC7 || Texture.Format == D3DFMT_ATI1 || Texture.Format == D3DFMT_ATI2)
		{
			unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RED_RGTC1) ? 8 : 16;
			unsigned int offset = 0;

			for (unsigned int level = 0; level < Texture.Levels; ++level)
			{
				unsigned int size = ((Texture.Width + 3) / 4)*((Texture.Height + 3) / 4)*blockSize;

				glCompressedTexImage2D(GL_TEXTURE_2D, level, format, Texture.Width, Texture.Height, 0, size, &file._buffer.p[Texture.DataPointer] + offset);

				offset += size;
				Texture.Width /= 2;
				Texture.Height /= 2;
			}

		}
		else {

			if (Texture.Format == D3DFMT_A8R8G8B8)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Texture.Width, Texture.Height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, &file._buffer.p[Texture.DataPointer]);
			if (Texture.Format == D3DFMT_A1R5G5B5)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, Texture.Width, Texture.Height, 0, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, &file._buffer.p[Texture.DataPointer]);
			if (Texture.Format == D3DFMT_A8) //NO CLUE?????
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Texture.Width, Texture.Height, 0, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, &file._buffer.p[Texture.DataPointer]);
			if (Texture.Format == D3DFMT_A8B8G8R8)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Texture.Width, Texture.Height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, &file._buffer.p[Texture.DataPointer]);
			if (Texture.Format == D3DFMT_L8)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, Texture.Width, Texture.Height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &file._buffer.p[Texture.DataPointer]);
		}

		TextureManager::LoadTexture(TextureNameHashes[i], textureID);

		file.seekg(posOriginal);

	}

}


YtdFile::~YtdFile()
{
	for (int i = 0; i < TextureNameHashes.size(); i++)
	{
		TextureManager::RemoveTexture(TextureNameHashes[i]);
	}
}
