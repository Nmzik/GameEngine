#pragma once
#include <cstdint>

enum VertexType : uint32_t
{
    Default = 89,       // PNCT
    DefaultEx = 16473,  //    PNCTX
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

struct VertexBufferHandle
{
    int32_t id;
};

struct IndexBufferHandle
{
    int32_t id;
};

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
};

class Geometry
{
    VertexBufferHandle vertexBufferHandle;
    IndexBufferHandle indexBufferHandle;
    uint32_t indexCount;

public:
    VertexType type;
    //Geometry(const uint8_t* meshData, grmGeometry* drawGeom, uint16_t _materialID, uint32_t Shader);
    Geometry(VertexBufferHandle vert_handle, IndexBufferHandle index_handle, uint32_t countIndices);
    ~Geometry();

    VertexBufferHandle getVertexBufferHandle() const
    {
        return vertexBufferHandle;
    }

    IndexBufferHandle getIndexBufferHandle() const
    {
        return indexBufferHandle;
    }

    uint32_t getIndexCount() const
    {
        return indexCount;
    }
};
