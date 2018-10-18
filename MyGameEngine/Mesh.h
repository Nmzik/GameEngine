#pragma once
#include "opengl.h"
#include "Material.h"
#include "MeshManager.h"

enum VertexType : uint32_t
{
	Default = 89, //PNCT
	DefaultEx = 16473, //PNCTX
	PNCCT = 121,
	PNCCTTTT = 1017,
	PCCNCCTTX = 16639,
	PCCNCCT = 127,
	PNCTTTX = 16857,
	PNCTTX = 16601,
	PNCTTTX_2 = 19545,
	PNCTTTX_3 = 17113,
	PNCCTTX = 16633,
	PNCCTTX_2 = 17017,
	PNCCTTTX = 17145,
	PCCNCCTX = 16511,
	PCCNCTX = 16479,
	PCCNCT = 95,
	PNCCTT = 249,
	PNCCTX = 16505,
	PCT = 81,
	PT = 65,
	PTT = 193,
	PNC = 25,
	PC = 17,
	PCC = 7,
	PCCH2H4 = 2147500121, //0x80004059  (16473 + 0x80000000) DefaultEx Cloth?
	PNCH2 = 2147483737, //0x80000059  (89 + 0x80000000) Default Cloth?
	PNCTTTTX = 19673,  //normal_spec_detail_dpm_vertdecal_tnt
	PNCTTTT = 985,
	PCCNCCTT = 255,
	PCTT = 209,
	PCCCCT = 119,
	PCCNC = 31,
	PCCNCTT = 223,
	PCCNCTTX = 16607,
	PCCNCTTT = 479,
	PNCTT = 217,
	PNCTTT = 473,
};

struct VertexDeclaration {
	uint32_t Flags;
	uint16_t Stride;
	uint8_t Unknown_6h;
	uint8_t Count;
	uint64_t Types;
};

struct VertexBuffer {
	uint32_t VFT;
	uint32_t Unknown_4h; // 0x00000001
	uint16_t VertexStride;
	uint16_t Unknown_Ah;
	uint32_t Unknown_Ch; // 0x00000000
	uint64_t DataPointer1;
	uint32_t VertexCount;
	uint32_t Unknown_1Ch; // 0x00000000
	uint64_t DataPointer2;
	uint32_t Unknown_28h; // 0x00000000
	uint32_t Unknown_2Ch; // 0x00000000
	VertexDeclaration* InfoPointer;
	uint32_t Unknown_38h; // 0x00000000
	uint32_t Unknown_3Ch; // 0x00000000
	uint32_t Unknown_40h; // 0x00000000
	uint32_t Unknown_44h; // 0x00000000
	uint32_t Unknown_48h; // 0x00000000
	uint32_t Unknown_4Ch; // 0x00000000
	uint32_t Unknown_50h; // 0x00000000
	uint32_t Unknown_54h; // 0x00000000
	uint32_t Unknown_58h; // 0x00000000
	uint32_t Unknown_5Ch; // 0x00000000
	uint32_t Unknown_60h; // 0x00000000
	uint32_t Unknown_64h; // 0x00000000
	uint32_t Unknown_68h; // 0x00000000
	uint32_t Unknown_6Ch; // 0x00000000
	uint32_t Unknown_70h; // 0x00000000
	uint32_t Unknown_74h; // 0x00000000
	uint32_t Unknown_78h; // 0x00000000
	uint32_t Unknown_7Ch; // 0x00000000
};

struct IndexBuffer {
	uint32_t VFT;
	uint32_t Unknown_4h; // 0x00000001
	uint32_t IndicesCount;
	uint32_t Unknown_Ch; // 0x00000000
	uint64_t IndicesPointer;
	uint32_t Unknown_18h; // 0x00000000
	uint32_t Unknown_1Ch; // 0x00000000
	uint32_t Unknown_20h; // 0x00000000
	uint32_t Unknown_24h; // 0x00000000
	uint32_t Unknown_28h; // 0x00000000
	uint32_t Unknown_2Ch; // 0x00000000
	uint32_t Unknown_30h; // 0x00000000
	uint32_t Unknown_34h; // 0x00000000
	uint32_t Unknown_38h; // 0x00000000
	uint32_t Unknown_3Ch; // 0x00000000
	uint32_t Unknown_40h; // 0x00000000
	uint32_t Unknown_44h; // 0x00000000
	uint32_t Unknown_48h; // 0x00000000
	uint32_t Unknown_4Ch; // 0x00000000
	uint32_t Unknown_50h; // 0x00000000
	uint32_t Unknown_54h; // 0x00000000
	uint32_t Unknown_58h; // 0x00000000
	uint32_t Unknown_5Ch; // 0x00000000
};

class Mesh
{
public:
	GLuint VBO, EBO, VAO;

	Material material;
	uint32_t num_indices;

	Mesh(const uint8_t* meshData, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Material mat);
	void Cleanup();

	~Mesh();
};

