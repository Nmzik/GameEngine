#pragma once
#include "FileType.h"
#include <unordered_map>
#include "opengl.h"

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

	//	UNKNOWN
};

struct TextureBase : datBase
{
	uint32_t Unknown_8h;  // 0x00000000
	uint32_t Unknown_Ch;  // 0x00000000
	uint32_t Unknown_10h; // 0x00000000
	uint32_t Unknown_14h; // 0x00000000
	uint32_t Unknown_18h; // 0x00000000
	uint32_t Unknown_1Ch; // 0x00000000
	uint32_t Unknown_20h; // 0x00000000
	uint32_t Unknown_24h; // 0x00000000
	uint64_t NamePointer;
	uint32_t Unknown_30h;
	uint32_t Unknown_34h; // 0x00000000
	uint32_t Unknown_38h; // 0x00000000
	uint32_t Unknown_3Ch; // 0x00000000
};

struct grcTexture : TextureBase
{
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

	void Resolve(memstream& file)
	{
		GRAPHICS_BASE_PTR(DataPointer);
	}
};

struct TextureDictionary : ResourceFileBase
{
	uint32_t Unknown_10h; // 0x00000000
	uint32_t Unknown_14h; // 0x00000000
	uint32_t Unknown_18h; // 0x00000001
	uint32_t Unknown_1Ch; // 0x00000000
	ResourceSimpleList64Ptr TextureNameHashesPtr;
	pgObjectArray<grcTexture> Textures;

	void Resolve(memstream& file)
	{
		Textures.Resolve(file);
	}
};

class YtdLoader : public FileType
{
public:
	YtdLoader* next;
	std::unordered_map<uint32_t, GLuint>* Textures = nullptr;

	void Init(memstream& file) override;
	void Remove();
};
