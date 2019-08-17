//
//  MetalRenderer.cpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#include "MetalRenderer.h"
#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"
#include "../YdrLoader.h"

#include <simd/simd.h>
#include "glm/glm.hpp"

//id <MTLDevice> device;
//MTKView* metalView;
#define gpuBufferSize 16000
id<MTLBuffer> vertexBuffers[gpuBufferSize] = {0};
id<MTLBuffer> indexBuffers[gpuBufferSize] = {0};
uint32_t curIndex = 0;

id<MTLCommandQueue> commandQueue;
MTLRenderPassDescriptor* mainDescriptor;
MTLRenderPipelineDescriptor* renderDescriptor;
id <MTLRenderPipelineState> renderPipelineState;

id<MTLRenderPipelineState> DefaultPipelineState;

id <MTLBuffer> sceneBuffer;

id <MTLBuffer> cubeTestBuffer;

id <MTLRenderCommandEncoder> commandEncoder;

float cubeVertices[] =
{
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};


struct uniform_buffer_struct {
    matrix_float4x4 projViewMatrix;
    matrix_float4x4 modelMatrix;
} scene_matrices;

MetalRenderer::MetalRenderer()
{
    device = MTLCreateSystemDefaultDevice();
    commandQueue = [device newCommandQueue];
    NSLog(@"METAL RENDERER IS READY!");
    
    id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
    
    renderDescriptor = [MTLRenderPipelineDescriptor new];
    renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
    renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
    //
    MTLVertexDescriptor* attribute = [MTLVertexDescriptor new];
    attribute.attributes[0].offset = 0;
    attribute.attributes[0].format = MTLVertexFormatFloat3;
    attribute.attributes[0].bufferIndex = 0;
    attribute.attributes[1].offset = 12;
    attribute.attributes[1].format = MTLVertexFormatFloat3;
    attribute.attributes[1].bufferIndex = 0;
    attribute.attributes[2].offset = 24;
    attribute.attributes[2].format = MTLVertexFormatUChar4;
    attribute.attributes[2].bufferIndex = 0;
    attribute.attributes[3].offset = 28;
    attribute.attributes[3].format = MTLVertexFormatFloat2;
    attribute.attributes[3].bufferIndex = 0;
    //
    attribute.layouts[0].stepRate = 1;
    attribute.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
    attribute.layouts[0].stride = 36;
    //
    renderDescriptor.vertexDescriptor = attribute;
    
    renderPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    
    mainDescriptor = [MTLRenderPassDescriptor new];
    mainDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    mainDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 104/255, 55/255, 1.0);
    mainDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    
    sceneBuffer = [device newBufferWithLength:sizeof(scene_matrices) options:MTLResourceStorageModeShared];
    
    NSLog(@"%lu",sizeof(cubeVertices));
    cubeTestBuffer = [device newBufferWithBytes:&cubeVertices[0] length:sizeof(cubeVertices)  options:MTLResourceStorageModeShared];
}

MetalRenderer::~MetalRenderer()
{
    
}

VertexBufferHandle MetalRenderer::createVertexBuffer(uint32_t size, const uint8_t* pointer)
{
    
    //PERFORMANCE!!!
    id<MTLBuffer> buffer = [device newBufferWithBytes:pointer length:size options:MTLResourceStorageModeShared];
    
    float testing[20];
    memcpy(&testing[0], pointer, sizeof(float) * 20);
    
    VertexBufferHandle handle;
    
    //find free
    BOOL found = false;
    for (int i = 0; i < gpuBufferSize; i++) {
        if (vertexBuffers[i] == 0)
        {
            vertexBuffers[i] = buffer;
            handle.id = i;
            found = true;
            break;
        }
    }
    
    assert(found);
    
    return handle;
}

IndexBufferHandle MetalRenderer::createIndexBuffer(uint32_t size, const uint8_t* pointer)
{
    //PERFORMANCE
    id<MTLBuffer> buffer = [device newBufferWithBytes:pointer length:size options:MTLResourceStorageModeShared];
    
    IndexBufferHandle handle;
    
    //find free
    BOOL found = false;
    for (int i = 0; i < gpuBufferSize; i++) {
        if (indexBuffers[i] == 0)
        {
            indexBuffers[i] = buffer;
            handle.id = i;
            found = true;
            break;
        }
    }
    
    assert(found);
    
    return handle;
}

void MetalRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
    assert(vertexBuffers[handle.id] != 0);
    vertexBuffers[handle.id] = 0;
}

void MetalRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
    assert(indexBuffers[handle.id] != 0);
    indexBuffers[handle.id] = 0;
}

template <typename T, typename U >
const U static inline convert( const T & t )
{
    U tmp;
    memcpy(&tmp, &t, sizeof(U));
    U ret = tmp;
    return ret;
}

const matrix_float4x4 static inline toMtl( const glm::mat4 & mat )
{
    return convert<glm::mat4, matrix_float4x4>(mat);
}

matrix_float4x4 identity()
{
    vector_float4 X = { 1, 0, 0, 0 };
    vector_float4 Y = { 0, 1, 0, 0 };
    vector_float4 Z = { 0, 0, 1, 0 };
    vector_float4 W = { 0, 0, 0, 1 };
    
    matrix_float4x4 identity = { X, Y, Z, W };
    
    return identity;
}

matrix_float4x4 translation(vector_float4 t)
{
    vector_float4 X = { 1, 0, 0, 0 };
    vector_float4 Y = { 0, 1, 0, 0 };
    vector_float4 Z = { 0, 0, 1, 0 };
    vector_float4 W = { t.x, t.y, t.z, t.w };
    
    matrix_float4x4 mat = { X, Y, Z, W };
    
    return mat;
}

matrix_float4x4 rotation_about_axis(vector_float3 axis, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    
    vector_float4 X;
    X.x = axis.x * axis.x + (1 - axis.x * axis.x) * c;
    X.y = axis.x * axis.y * (1 - c) - axis.z*s;
    X.z = axis.x * axis.z * (1 - c) + axis.y * s;
    X.w = 0.0;
    
    vector_float4 Y;
    Y.x = axis.x * axis.y * (1 - c) + axis.z * s;
    Y.y = axis.y * axis.y + (1 - axis.y * axis.y) * c;
    Y.z = axis.y * axis.z * (1 - c) - axis.x * s;
    Y.w = 0.0;
    
    vector_float4 Z;
    Z.x = axis.x * axis.z * (1 - c) - axis.y * s;
    Z.y = axis.y * axis.z * (1 - c) + axis.x * s;
    Z.z = axis.z * axis.z + (1 - axis.z * axis.z) * c;
    Z.w = 0.0;
    
    vector_float4 W;
    W.x = 0.0;
    W.y = 0.0;
    W.z = 0.0;
    W.w = 1.0;
    
    matrix_float4x4 mat = { X, Y, Z, W };
    return mat;
}

matrix_float4x4 perspective_projection(float aspect, float fovy, float near, float far)
{
    float yScale = 1 / tan(fovy * 0.5);
    float xScale = yScale / aspect;
    float zRange = far - near;
    float zScale = -(far + near) / zRange;
    float wzScale = -2 * far * near / zRange;
    
    vector_float4 P = { xScale, 0, 0, 0 };
    vector_float4 Q = { 0, yScale, 0, 0 };
    vector_float4 R = { 0, 0, zScale, -1 };
    vector_float4 S = { 0, 0, wzScale, 0 };
    
    matrix_float4x4 mat = { P, Q, R, S };
    return mat;
}

void MetalRenderer::renderDrawable(YdrLoader* drawable)
{
    int curTexture = 0;
    for (auto& model : drawable->models)
    {
        if ((model.Unk_2Ch & 1) == 0)
        {
            continue;  //    PROXIES
        }
        for (auto& geometry : model.geometries)
        {
            if (geometry.type != VertexType::Default)
                continue;
            
            //set renderPIPELINE STATE
            [commandEncoder setRenderPipelineState:renderPipelineState];
            
            //[commandEncoder setVertexBuffer:cubeTestBuffer offset:0 atIndex:0];
            [commandEncoder setVertexBuffer:vertexBuffers[geometry.getVertexBufferHandle().id] offset:0 atIndex:0];
            memcpy([sceneBuffer contents], &scene_matrices, sizeof(uniform_buffer_struct));
            
            [commandEncoder setVertexBuffer:sceneBuffer offset:0 atIndex:1];
            
            [commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:geometry.getIndexCount() indexType:MTLIndexTypeUInt16 indexBuffer:indexBuffers[geometry.getIndexBufferHandle().id] indexBufferOffset:0];
            
            //[commandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:12 * 3];
            
            /*[commandEncoder setVertexBuffer:vertexBuffers[geometry.getVertexBufferHandle().id] offset:0 atIndex:0]; //NOT SURE
             [commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:geometry.getIndexCount() indexType:MTLIndexTypeUInt16 indexBuffer:indexBuffers[geometry.getIndexBufferHandle().id] indexBufferOffset:0];*/
            
            //numDrawCalls++;
        }
    }
}

void MetalRenderer::renderBuilding(CBuilding* building)
{
    renderDrawable(building->ydr);
}

void MetalRenderer::renderPed(CPed* ped)
{
    for (auto& ydr : ped->playerModel)
    {
        renderDrawable(ydr);
    }
}

void MetalRenderer::renderVehicle(CVehicle* vehicle)
{
    renderDrawable(vehicle->getDrawable()->ydr);
}

void MetalRenderer::renderWorld(GameWorld* world, Camera* curCamera)
{
    glm::vec3 playerPos = world->getCurrentPlayer()->getPosition();
    NSLog(@"%f %f %f", playerPos.x, playerPos.y, playerPos.z);
    //NSLog(@"%f %f %f", curCamera->getPosition().x, curCamera->getPosition().y, curCamera->getPosition().z);
    //curCamera->setPosition(curCamera->getPosition() + glm::vec3(0,0, -1));
    
    glm::mat4 view = curCamera->getViewMatrix();
    glm::mat4 projection = curCamera->getProjection();
    glm::mat4 projectionView = projection * view;
    
    static const vector_float4 cameraPosition = { 0, 0, -4, 1 };
    const CGSize size = CGSizeMake(1280, 720);
    const CGFloat aspectRatio = size.width / size.height;
    const CGFloat verticalFOV = (aspectRatio > 1) ? 60 : 90;
    static const CGFloat near = 0.1;
    static const CGFloat far = 100;
    
    matrix_float4x4 projectionMatrix = perspective_projection(aspectRatio, verticalFOV * (M_PI / 180), near, far);
    
    glm::mat4 tet = glm::perspective(verticalFOV * (M_PI / 180), aspectRatio, near, far);
    
    matrix_float4x4 modelMatrix = identity();
    
    matrix_float4x4 worldCameraPosition = toMtl(view);
    //
    curCamera->updateFrustum(projectionView); //maybe remove this??? opengl ONLY
    
    auto drawable = [mtkView currentDrawable];
    
    mainDescriptor.colorAttachments[0].texture = drawable.texture;
    
    id <MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:mainDescriptor];
    
    scene_matrices.projViewMatrix = matrix_multiply(projectionMatrix, worldCameraPosition);
    scene_matrices.modelMatrix = identity();
    
    [commandEncoder setRenderPipelineState:renderPipelineState];
    
    //[commandEncoder setVertexBuffer:cubeTestBuffer offset:0 atIndex:0];
    [commandEncoder setVertexBuffer:vertexBuffers[0] offset:0 atIndex:0];
    memcpy([sceneBuffer contents], &scene_matrices, sizeof(uniform_buffer_struct));
    
    [commandEncoder setVertexBuffer:sceneBuffer offset:0 atIndex:1];
    
    [commandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:36];
    
    /*for (auto& object : world->renderList)
    {
        //CHECK WITH GLKIT rotation position
        scene_matrices.modelMatrix = identity();
        //scene_matrices.modelMatrix = toMtl(object->getMatrix());
        
        switch (object->getType())
        {
            case ObjectType::Building:
            {
                CBuilding* building = static_cast<CBuilding*>(object);
                renderBuilding(building);
                break;
            }
            case ObjectType::Vehicle:
            {
                CVehicle* vehicle = static_cast<CVehicle*>(object);
                renderVehicle(vehicle);
                break;
            }
            case ObjectType::Ped:
            {
                CPed* ped = static_cast<CPed*>(object);
                renderPed(ped);
                break;
            }
            default:
                break;
        }
    }*/
    
    [commandEncoder endEncoding];
    
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}
