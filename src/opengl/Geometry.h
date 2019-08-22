#pragma once
#include "Windows/Win32Window.h"

enum VertexType : uint32_t
{
    Default = 89,       // PNCT
    DefaultEx = 16473,  //	PNCTX
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
    PCCH2H4 = 2147500121,  // 0x80004059  (16473 + 0x80000000) DefaultEx Cloth?
    PNCH2 = 2147483737,    // 0x80000059  (89 + 0x80000000) Default Cloth?
    PNCTTTTX = 19673,      // normal_spec_detail_dpm_vertdecal_tnt
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

struct VertexAttributes
{
    GLuint index;
    GLint size;
    GLenum type;
    GLsizei stride;
    const void* pointer;
};

const std::vector<VertexAttributes> DefaultAttrib = {
    {0, 3, GL_FLOAT, 36, nullptr},
    {1, 3, GL_FLOAT, 36, (GLvoid*)12},
    {2, 4, GL_UNSIGNED_BYTE, 36, (GLvoid*)24},
    {3, 2, GL_FLOAT, 36, (GLvoid*)28}};

const std::vector<VertexAttributes> DefaultExAttrib = {
    {0, 3, GL_FLOAT, 52, nullptr},
    {1, 3, GL_FLOAT, 52, (GLvoid*)12},
    {2, 4, GL_UNSIGNED_BYTE, 52, (GLvoid*)24},
    {3, 2, GL_FLOAT, 52, (GLvoid*)28}};

const std::vector<VertexAttributes> PNCCTAttrib = {
    {0, 3, GL_FLOAT, 40, nullptr}, {1, 3, GL_FLOAT, 40, (GLvoid*)12}, {2, 4, GL_UNSIGNED_BYTE, 40, (GLvoid*)24}, {3, 2, GL_FLOAT, 40, (GLvoid*)32}};
const std::vector<VertexAttributes> PNCCTTTTAttrib = {
    {0, 3, GL_FLOAT, 64, nullptr}, {1, 3, GL_FLOAT, 64, (GLvoid*)12}, {2, 4, GL_UNSIGNED_BYTE, 64, (GLvoid*)24}, {3, 2, GL_FLOAT, 64, (GLvoid*)32}};
const std::vector<VertexAttributes> PCCNCCTTXAttrib = {
    {0, 3, GL_FLOAT, 72, nullptr}, {1, 3, GL_FLOAT, 72, (GLvoid*)20}, {2, 4, GL_UNSIGNED_BYTE, 72, (GLvoid*)12}, {3, 2, GL_FLOAT, 72, (GLvoid*)40}};
const std::vector<VertexAttributes> PCCNCCTAttrib = {
    {0, 3, GL_FLOAT, 48, nullptr}, {1, 3, GL_FLOAT, 48, (GLvoid*)20}, {2, 4, GL_UNSIGNED_BYTE, 48, (GLvoid*)12}, {3, 2, GL_FLOAT, 48, (GLvoid*)40}};
const std::vector<VertexAttributes> PNCTTTXAttrib = {
    {0, 3, GL_FLOAT, 68, nullptr}, {1, 3, GL_FLOAT, 68, (GLvoid*)12}, {2, 4, GL_UNSIGNED_BYTE, 68, (GLvoid*)24}, {3, 2, GL_FLOAT, 68, (GLvoid*)28}};
const std::vector<VertexAttributes> PNCTTXAttrib = {
    {0, 3, GL_FLOAT, 60, nullptr}, {1, 3, GL_FLOAT, 60, (GLvoid*)12}, {3, 2, GL_FLOAT, 60, (GLvoid*)28}};
const std::vector<VertexAttributes> PNCTTTX_2Attrib = {
    {0, 3, GL_FLOAT, 68, nullptr}, {1, 3, GL_FLOAT, 68, (GLvoid*)12}, {3, 2, GL_FLOAT, 68, (GLvoid*)28}};
const std::vector<VertexAttributes> PNCTTTX_3Attrib = {
    {0, 3, GL_FLOAT, 68, nullptr}, {1, 3, GL_FLOAT, 68, (GLvoid*)12}, {3, 2, GL_FLOAT, 68, (GLvoid*)28}};
const std::vector<VertexAttributes> PNCCTTXAttrib = {
    {0, 3, GL_FLOAT, 64, nullptr}, {1, 3, GL_FLOAT, 64, (GLvoid*)12}, {3, 2, GL_FLOAT, 64, (GLvoid*)32}};
const std::vector<VertexAttributes> PNCCTTX_2Attrib = {
    {0, 3, GL_FLOAT, 64, nullptr}, {1, 3, GL_FLOAT, 64, (GLvoid*)12}, {3, 2, GL_FLOAT, 64, (GLvoid*)32}};
const std::vector<VertexAttributes> PNCCTTTXAttrib = {
    {0, 3, GL_FLOAT, 72, nullptr}, {1, 3, GL_FLOAT, 72, (GLvoid*)12}, {3, 2, GL_FLOAT, 72, (GLvoid*)32}};
const std::vector<VertexAttributes> PCCNCCTXAttrib = {
    {0, 3, GL_FLOAT, 64, nullptr}, {1, 3, GL_FLOAT, 64, (GLvoid*)20}, {3, 2, GL_FLOAT, 64, (GLvoid*)40}};
const std::vector<VertexAttributes> PCCNCTXAttrib = {
    {0, 3, GL_FLOAT, 60, nullptr}, {1, 3, GL_FLOAT, 60, (GLvoid*)20}, {3, 2, GL_FLOAT, 60, (GLvoid*)36}};
const std::vector<VertexAttributes> PCCNCTAttrib = {
    {0, 3, GL_FLOAT, 44, nullptr}, {1, 3, GL_FLOAT, 44, (GLvoid*)20}, {3, 2, GL_FLOAT, 44, (GLvoid*)36}};
const std::vector<VertexAttributes> PNCCTTAttrib = {
    {0, 3, GL_FLOAT, 48, nullptr}, {1, 3, GL_FLOAT, 48, (GLvoid*)12}, {3, 2, GL_FLOAT, 48, (GLvoid*)32}};
const std::vector<VertexAttributes> PNCCTXAttrib = {
    {0, 3, GL_FLOAT, 56, nullptr}, {1, 3, GL_FLOAT, 56, (GLvoid*)12}, {3, 2, GL_FLOAT, 56, (GLvoid*)32}};
const std::vector<VertexAttributes> PCTAttrib = {{0, 3, GL_FLOAT, 24, nullptr}, {2, 3, GL_FLOAT, 24, (GLvoid*)12}, {3, 2, GL_FLOAT, 24, (GLvoid*)16}};
const std::vector<VertexAttributes> PTAttrib = {{0, 3, GL_FLOAT, 20, nullptr}, {3, 2, GL_FLOAT, 20, (GLvoid*)12}};
const std::vector<VertexAttributes> PTTAttrib = {{0, 3, GL_FLOAT, 28, nullptr}, {3, 3, GL_FLOAT, 28, (GLvoid*)12}};
const std::vector<VertexAttributes> PNCAttrib = {
    {0, 3, GL_FLOAT, 28, nullptr},
    {1, 3, GL_FLOAT, 28, (GLvoid*)12},
    {2, 2, GL_FLOAT, 28, (GLvoid*)24},
};
const std::vector<VertexAttributes> PCAttrib = {{0, 3, GL_FLOAT, 16, nullptr}};
const std::vector<VertexAttributes> PCCAttrib = {{0, 3, GL_FLOAT, 20, nullptr}};
const std::vector<VertexAttributes> PCCH2H4Attrib = {
    {0, 3, GL_FLOAT, 32, nullptr},
    {1, 4, GL_BYTE, 32, (GLvoid*)12},
    {2, 4, GL_UNSIGNED_BYTE, 32, (GLvoid*)16},
    {3, 2, GL_HALF_FLOAT, 32, (GLvoid*)20},  //	FIXED
};
const std::vector<VertexAttributes> PNCH2Attrib = {
    {0, 3, GL_FLOAT, 32, nullptr},
    {1, 3, GL_FLOAT, 32, (GLvoid*)12},
    {2, 4, GL_UNSIGNED_BYTE, 32, (GLvoid*)24},
    {3, 2, GL_HALF_FLOAT, 32, (GLvoid*)28}  //	FIXED
};
const std::vector<VertexAttributes> PNCTTTTXAttrib = {
    {0, 3, GL_FLOAT, 76, nullptr}, {1, 3, GL_FLOAT, 76, (GLvoid*)12}, {2, 4, GL_UNSIGNED_BYTE, 76, (GLvoid*)24}, {3, 2, GL_FLOAT, 76, (GLvoid*)28}};
/*const std::vector<VertexAttributes> PNCTTTTAttrib = {
};
const std::vector<VertexAttributes> PCCNCCTTAttrib = {
};
const std::vector<VertexAttributes> PCTTAttrib = {
};
const std::vector<VertexAttributes> PCCCCTAttrib = {
};
const std::vector<VertexAttributes> PCCNCAttrib = {
};*/
const std::vector<VertexAttributes> PCCNCTTAttrib = {
    {0, 3, GL_FLOAT, 52, nullptr},
    {1, 3, GL_FLOAT, 52, (GLvoid*)20},
    {2, 4, GL_UNSIGNED_BYTE, 52, (GLvoid*)12},
    {3, 2, GL_FLOAT, 52, (GLvoid*)36},
};
const std::vector<VertexAttributes> PCCNCTTXAttrib = {
    {0, 3, GL_FLOAT, 68, nullptr},
    {1, 3, GL_FLOAT, 68, (GLvoid*)20},
    {2, 4, GL_UNSIGNED_BYTE, 68, (GLvoid*)12},
    {3, 2, GL_FLOAT, 68, (GLvoid*)36},
};
const std::vector<VertexAttributes> PCCNCTTTAttrib = {
    {0, 3, GL_FLOAT, 60, nullptr},
    {1, 3, GL_FLOAT, 60, (GLvoid*)20},
    {2, 4, GL_UNSIGNED_BYTE, 60, (GLvoid*)12},
    {3, 2, GL_FLOAT, 60, (GLvoid*)36},
};

const std::vector<VertexAttributes> WaterAttrib = {{0, 3, GL_FLOAT, 5 * sizeof(float), nullptr},
                                                   {1, 3, GL_FLOAT, 5 * sizeof(float), (GLvoid*)nullptr},
                                                   {2, 2, GL_FLOAT, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float))}};

struct grmGeometry;

struct Material
{
    //Material(uint32_t DiffuseHash, uint32_t BumpHash, uint32_t SpecularHash, uint32_t DetailHash) {
    /*diffuseTextureID = TextureManager::GetTextureManager().GetTexture(DiffuseHash);
    if (BumpHash != 0)
    {
        bumpTextureID = TextureManager::GetTextureManager().GetTexture(BumpHash);
        useBump = true;
    }
    if (SpecularHash != 0)
        specularTextureID = TextureManager::GetTextureManager().GetTexture(SpecularHash);
    if (DetailHash != 0)
        detailTextureID = TextureManager::GetTextureManager().GetTexture(SpecularHash);*/
    //}
    //~Material();

    bool useBump = false;

    GLuint diffuseTextureID;
    GLuint bumpTextureID;  //	Normal
    GLuint specularTextureID;
    GLuint detailTextureID;
};

class Geometry
{
public:
    GLuint VBO[2], VAO;

    uint16_t materialID;
    uint32_t num_indices;
    uint32_t ShaderName;

    Geometry(const uint8_t* meshData, grmGeometry* drawGeom, uint16_t _materialID, uint32_t Shader);
    ~Geometry();
};