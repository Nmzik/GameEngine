#pragma once

#include <stack>

#include "Geometry.h"
#include "Texture.h"
#include "TextureManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

class GameWorld;
class Camera;
class Shader;
//
class YdrLoader;
class CBuilding;
class CPed;
class CVehicle;

enum class Attribute
{
    UINT8 = 0,
    HALFFLOAT = 1,
    FLOAT = 2
};

struct VertexAttributes
{
    int index;
    int size;
    Attribute type;
    int offset;
};

struct VertexLayout
{
    VertexAttributes attributes[5];
    int stride;
    int size;
};

static VertexLayout Default_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    36,
    4};

static VertexLayout DefaultEx_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    52,
    4};

static VertexLayout PNCCT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 32}},
    40,
    4};
static VertexLayout PNCCTTTT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 32}},
    64,
    4};
static VertexLayout PBBNCCTTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {2, 4, Attribute::UINT8, 32},
     {3, 2, Attribute::FLOAT, 40}},
    72,
    4};
static VertexLayout PBBNCCT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {2, 4, Attribute::UINT8, 12},
     {3, 2, Attribute::FLOAT, 40}},
    48,
    4};
static VertexLayout PNCTTTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    68,
    4};
static VertexLayout PNCTTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 28}},
    60,
    3};
static VertexLayout PNCTTTX_2_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 28}},
    68,
    3};
static VertexLayout PNCTTTX_3_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 28}},
    68,
    3};
static VertexLayout PNCCTTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    64,
    3};
static VertexLayout PNCCTTX_2_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    64,
    3};
static VertexLayout PNCCTTTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    72,
    3};
static VertexLayout PBBNCCTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {3, 2, Attribute::FLOAT, 40}},
    64,
    3};
static VertexLayout PBBNCTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {3, 2, Attribute::FLOAT, 36}},
    60,
    3};
static VertexLayout PBBNCT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {3, 2, Attribute::FLOAT, 36}},
    44,
    3};
static VertexLayout PNCCTT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    48,
    3};
static VertexLayout PNCCTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    56,
    3};
static VertexLayout PCT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 2, Attribute::FLOAT, 16}},
    24,
    3};
static VertexLayout PT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 0},
     {3, 2, Attribute::FLOAT, 12}},
    20,
    3};
static VertexLayout PTT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 0},
     {3, 3, Attribute::FLOAT, 12}},
    28,
    3};
static VertexLayout PNC_Attrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 0},
        {3, 2, Attribute::FLOAT, 20},  //COLOR
    },
    28,
    3};
static VertexLayout PC_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 0},
     {3, 2, Attribute::FLOAT, 8}},
    16,
    3};
static VertexLayout PCC_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 0},
     {3, 2, Attribute::FLOAT, 12}},
    20,
    3};
static VertexLayout PCCH2H4_Attrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 4, Attribute::UINT8, 12},
        {2, 4, Attribute::UINT8, 16},
        {3, 2, Attribute::HALFFLOAT, 20},
    },
    32,
    4};
static VertexLayout PNCH2_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::HALFFLOAT, 28}},
    32,
    4};
static VertexLayout PNCTTTTX_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    76,
    4};
static VertexLayout PNCTTTT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    60,
    4};
static VertexLayout PBBNCCTT_Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {2, 4, Attribute::UINT8, 32},
     {3, 2, Attribute::FLOAT, 40}},
    56,
    4};
/*static VertexLayout PCTT_Attrib = {
 };*/
/*static VertexLayout PCCNC_Attrib = {
 };*/
static VertexLayout PBBNCTT_Attrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 20},
        {2, 4, Attribute::UINT8, 12},
        {3, 2, Attribute::FLOAT, 36},
    },
    52,
    4};
static VertexLayout PBBNCTTX_Attrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 20},
        {2, 4, Attribute::UINT8, 12},
        {3, 2, Attribute::FLOAT, 36},
    },
    68,
    4};
static VertexLayout PBBNCTTT_Attrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 20},
        {2, 4, Attribute::UINT8, 12},
        {3, 2, Attribute::FLOAT, 36},
    },
    60,
    4};

#define gpuBufferSize 18000

class BaseRenderer
{
    int numDrawCalls;

    TextureManager textureManager;

public:
    BaseRenderer(/*NativeWindow* window*/);
    virtual ~BaseRenderer();

    bool renderDebugWorld;
    uint64_t GlobalGpuMemory = 0;
    uint64_t TextureMemory = 0;

    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) = 0;
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) = 0;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) = 0;
    virtual uint32_t getLayoutHandle(VertexType type) = 0;

    virtual void removeTexture(TextureHandle handle) = 0;
    virtual void removeVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void removeIndexbuffer(IndexBufferHandle handle) = 0;
    virtual void updateGlobalSceneBuffer(glm::mat4& Projection, glm::mat4& View) = 0;
    virtual void updatePerModelData(glm::mat4& mat) = 0;
    virtual void renderGeom(Geometry& geom) = 0;
    /*void beginFrame();
     void endFrame();*/
    virtual void beginFrame() = 0;
    virtual void presentFrame() = 0;
    //
    TextureManager* getTextureManager()
    {
        return &textureManager;
    }

    int getNumDrawCalls() const
    {
        return numDrawCalls;
    }

protected:
    std::unordered_map<VertexType, uint32_t> layoutHandles;
};
