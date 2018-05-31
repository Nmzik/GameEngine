#pragma once

#include <vector>
#include "glm.hpp"
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

class Mesh
{
public:
	GLuint VBO, EBO, VAO;

	Material material;
	unsigned int num_indices;

	Mesh(const uint8_t* meshData, uint64_t VertexPointer, uint32_t VertexSize, uint64_t IndicesPointer, uint32_t IndicesSize, VertexType type, Material mat);
	void Cleanup();

	~Mesh();

	void Draw(Shader* shader);
};

