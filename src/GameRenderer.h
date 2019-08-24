#pragma once

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

struct VertexAttributes
{
    int index;
    int size;
    int type;
    int offset;
};

struct VertexLayout
{
    VertexAttributes attributes[5];
    int stride;
    int size;
};

enum Attribute
{
    UINT8 = 0,
    HALFFLOAT = 1,
    FLOAT = 2
};

static VertexLayout DefaultAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    36,
    4};

static VertexLayout DefaultExAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    52,
    4};

static VertexLayout PNCCTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 32}},
    40,
    4};
static VertexLayout PNCCTTTTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 32}},
    64,
    4};
static VertexLayout PCCNCCTTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {2, 4, Attribute::UINT8, 12},
     {3, 2, Attribute::FLOAT, 40}},
    72,
    4};
static VertexLayout PCCNCCTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {2, 4, Attribute::UINT8, 12},
     {3, 2, Attribute::FLOAT, 40}},
    48,
    4};
static VertexLayout PNCTTTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    68,
    4};
static VertexLayout PNCTTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 28}},
    60,
    3};
static VertexLayout PNCTTTX_2Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 28}},
    68,
    3};
static VertexLayout PNCTTTX_3Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 28}},
    68,
    3};
static VertexLayout PNCCTTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    64,
    3};
static VertexLayout PNCCTTX_2Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    64,
    3};
static VertexLayout PNCCTTTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    72,
    3};
static VertexLayout PCCNCCTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {3, 2, Attribute::FLOAT, 40}},
    64,
    3};
static VertexLayout PCCNCTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {3, 2, Attribute::FLOAT, 36}},
    60,
    3};
static VertexLayout PCCNCTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 20},
     {3, 2, Attribute::FLOAT, 36}},
    44,
    3};
static VertexLayout PNCCTTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    48,
    3};
static VertexLayout PNCCTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 32}},
    56,
    3};
static VertexLayout PCTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {2, 3, Attribute::FLOAT, 12},
     {3, 2, Attribute::FLOAT, 16}},
    24,
    3};
static VertexLayout PTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {3, 2, Attribute::FLOAT, 12}},
    20,
    2};
static VertexLayout PTTAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {3, 3, Attribute::FLOAT, 12}},
    28,
    2};
static VertexLayout PNCAttrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 12},
        {3, 2, Attribute::FLOAT, 20},  //COLOR
    },
    28,
    3};
static VertexLayout PCAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {3, 2, Attribute::FLOAT, 8}},
    16,
    2};
static VertexLayout PCCAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {3, 2, Attribute::FLOAT, 12}},
    20,
    2};
static VertexLayout PCCH2H4Attrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 4, Attribute::UINT8, 12},
        {2, 4, Attribute::UINT8, 16},
        {3, 2, Attribute::HALFFLOAT, 20},
    },
    32,
    4};
static VertexLayout PNCH2Attrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::HALFFLOAT, 28}},
    32,
    4};
static VertexLayout PNCTTTTXAttrib = {
    {{0, 3, Attribute::FLOAT, 0},
     {1, 3, Attribute::FLOAT, 12},
     {2, 4, Attribute::UINT8, 24},
     {3, 2, Attribute::FLOAT, 28}},
    76,
    4};
/*static VertexLayout PNCTTTTAttrib = {
 };
 static VertexLayout PCCNCCTTAttrib = {
 };
 static VertexLayout PCTTAttrib = {
 };
 static VertexLayout PCCCCTAttrib = {
 };
 static VertexLayout PCCNCAttrib = {
 };*/
static VertexLayout PCCNCTTAttrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 20},
        {2, 4, Attribute::UINT8, 12},
        {3, 2, Attribute::FLOAT, 36},
    },
    52,
    4};
static VertexLayout PCCNCTTXAttrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 20},
        {2, 4, Attribute::UINT8, 12},
        {3, 2, Attribute::FLOAT, 36},
    },
    68,
    4};
static VertexLayout PCCNCTTTAttrib = {
    {
        {0, 3, Attribute::FLOAT, 0},
        {1, 3, Attribute::FLOAT, 20},
        {2, 4, Attribute::UINT8, 12},
        {3, 2, Attribute::FLOAT, 36},
    },
    60,
    4};

#define gpuBufferSize 18000

class GameRenderer
{
    int numDrawCalls;

    TextureManager textureManager;

public:
    GameRenderer(/*NativeWindow* window*/);
    virtual ~GameRenderer();

    bool renderDebugWorld;

    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) = 0;
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) = 0;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) = 0;

    virtual void removeTexture(TextureHandle handle) = 0;
    virtual void removeVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void removeIndexbuffer(IndexBufferHandle handle) = 0;
    /*void beginFrame();
     void endFrame();
     void presentFrame();
     
     void renderDrawable(YdrLoader* drawable);
     void renderBuilding(CBuilding* building);
     void renderPed(CPed* ped);
     void renderVehicle(CVehicle* vehicle);*/
    //
    TextureManager* getTextureManager()
    {
        return &textureManager;
    }

    virtual void renderWorld(GameWorld* world, Camera* curCamera) = 0;

    int getNumDrawCalls() const
    {
        return numDrawCalls;
    }

    //
};
