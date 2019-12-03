#pragma once
#include <cstdint>

#include "Texture.h"

enum Hash : uint32_t
{
    DiffuseSampler = 4059966321,
    PlateBgSampler = 1342317448,
};

enum VertexType : uint32_t
{
    Default = 89,       //PNCT
    DefaultEx = 16473,  //PNCTX
    PNCCT = 121,
    PNCCTTTT = 1017,
    PBBNCCTTX = 16639,
    PBBNCCT = 127,
    PNCTTTX = 16857,
    PNCTTX = 16601,
    PNCTTTX_2 = 19545,
    PNCTTTX_3 = 17113,
    PNCCTTX = 16633,
    PNCCTTX_2 = 17017,
    PNCCTTTX = 17145,
    PBBNCCTX = 16511,
    PBBNCTX = 16479,
    PBBNCT = 95,
    PNCCTT = 249,
    PNCCTX = 16505,
    PCT = 81,
    PT = 65,
    PTT = 193,
    PNC = 25,
    PC = 17,
    PCC = 7,
    PCCH2H4 = 2147500121,  //0x80004059  (16473 + 0x80000000) DefaultEx Cloth?
    PNCH2 = 2147483737,    //0x80000059  (89 + 0x80000000) Default Cloth?
    PNCTTTTX = 19673,      //normal_spec_detail_dpm_vertdecal_tnt
    PNCTTTT = 985,
    PBBNCCTT = 255,
    PCTT = 209,
    PBBCCT = 119,
    PBBNC = 31,
    PBBNCTT = 223,
    PBBNCTTX = 16607,
    PBBNCTTT = 479,
    PNCTT = 217,
    PNCTTT = 473,
    PBBNCTTTX = 16863,
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
    uint32_t DiffuseSampler;
    uint32_t BumpSampler;
    uint32_t SpecularSampler;
    uint32_t DetailSampler;
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

    //bool useBump = false;
};

class Geometry
{
    VertexBufferHandle vertexBufferHandle;
    IndexBufferHandle indexBufferHandle;
    TextureHandle textureHandle;
    uint32_t indexCount;
    uint32_t vertexLayoutHandle;

public:
    //Geometry(const uint8_t* meshData, grmGeometry* drawGeom, uint16_t _materialID, uint32_t Shader);
    Geometry(VertexBufferHandle vert_handle, IndexBufferHandle index_handle, uint32_t layoutHandle, uint32_t countIndices, TextureHandle tex_handle);
    ~Geometry();

    VertexBufferHandle getVertexBufferHandle() const
    {
        return vertexBufferHandle;
    }

    IndexBufferHandle getIndexBufferHandle() const
    {
        return indexBufferHandle;
    }

    TextureHandle getTextureHandle() const
    {
        return textureHandle;
    }

    void setTextureHandle(TextureHandle& handle)
    {
        textureHandle = handle;
    }

    uint32_t getIndexCount() const
    {
        return indexCount;
    }

    uint32_t getVertexLayoutHandle() const
    {
        return vertexLayoutHandle;
    }
};
