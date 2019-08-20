//
//  MetalRenderer.cpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#include "MetalRenderer.h"

#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"
#include "../YdrLoader.h"
#include "TextureDecompressor.h"

#include <simd/simd.h>
#include "glm/glm.hpp"

uint32_t texturesSize = 0;
static int testID = 0;
#define gpuBufferSize 16000
id<MTLBuffer> vertexBuffers[gpuBufferSize] = {0};
id<MTLBuffer> indexBuffers[gpuBufferSize] = {0};
id<MTLTexture> textures[gpuBufferSize] = {0};
uint32_t curIndex = 0;

id<MTLCommandQueue> commandQueue;
MTLRenderPassDescriptor* mainPassDescriptor;
//
id<MTLRenderPipelineState> DefaultPipelineState;
id<MTLRenderPipelineState> DefaultExPipelineState;
id<MTLRenderPipelineState> PNCCTPipelineState;
id<MTLRenderPipelineState> PNCCTTTTPipelineState;
id<MTLRenderPipelineState> PCCNCCTTXPipelineState;
id<MTLRenderPipelineState> PCCNCCTPipelineState;
id<MTLRenderPipelineState> PNCTTTXPipelineState;
id<MTLRenderPipelineState> PNCTTXPipelineState;
id<MTLRenderPipelineState> PNCTTTX_2PipelineState;
id<MTLRenderPipelineState> PNCTTTX_3PipelineState;
id<MTLRenderPipelineState> PNCCTTXPipelineState;
id<MTLRenderPipelineState> PNCCTTX_2PipelineState;
id<MTLRenderPipelineState> PNCCTTTXPipelineState;
id<MTLRenderPipelineState> PCCNCCTXPipelineState;
id<MTLRenderPipelineState> PCCNCTXPipelineState;
id<MTLRenderPipelineState> PCCNCTPipelineState;
id<MTLRenderPipelineState> PNCCTTPipelineState;
id<MTLRenderPipelineState> PNCCTXPipelineState;
id<MTLRenderPipelineState> PCTPipelineState;
id<MTLRenderPipelineState> PTPipelineState;
id<MTLRenderPipelineState> PTTPipelineState;
id<MTLRenderPipelineState> PNCPipelineState;
id<MTLRenderPipelineState> PCPipelineState;
id<MTLRenderPipelineState> PCCPipelineState;
id<MTLRenderPipelineState> PCCH2H4PipelineState;
id<MTLRenderPipelineState> PNCH2PipelineState;
id<MTLRenderPipelineState> PNCTTTTXPipelineState;
//
id<MTLSamplerState> samplerState;
//id<MTLRenderPipelineState> PTTPipelineState;
id <MTLBuffer> scene_buffer;
id <MTLBuffer> cubeTestBuffer;
id <MTLRenderCommandEncoder> commandEncoder;
id <MTLDepthStencilState> depthStencilState;

//id <MTLHeap> heap;

//id<MTLBuffer> tempBuffer;

//id <MTLFence> fence;

glm::vec3 positions[] =
{
    {0, 0, 100},
    {-50, 50, 50},
    {-100, 100, 50},
    {-70, 70, 50},
    {70, -70, 50},
    {-200, 400, 50},
    {-400, 200, 10},
    {600, 200, 10},
};

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
    textureDecompressedMem = std::make_unique<uint8_t[]>(20 * 1024 * 1024); //20mb for uncompressing textures
}

MetalRenderer::~MetalRenderer()
{
    
}

void MetalRenderer::initializeEngine()
{
    commandQueue = [device newCommandQueue];
    
    createRenderPipelines();
    //
    mainPassDescriptor = [MTLRenderPassDescriptor new];
    mainPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    mainPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 104/255, 55/255, 1.0);
    mainPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    mainPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    mainPassDescriptor.depthAttachment.clearDepth = 1.0f;
    //mainPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    //mainPassDescriptor.depthAttachment.clearDepth = 1.0f;
    
    scene_buffer = [device newBufferWithLength:sizeof(uniform_buffer_struct) options:MTLResourceStorageModeShared];
    
    cubeTestBuffer = [device newBufferWithBytes:&cubeVertices[0] length:sizeof(cubeVertices)  options:MTLResourceStorageModeShared];
    
    MTLSamplerDescriptor *samplerDescriptor = [MTLSamplerDescriptor new];
    samplerDescriptor.minFilter = MTLSamplerMinMagFilterNearest;
    samplerDescriptor.magFilter = MTLSamplerMinMagFilterLinear;
    samplerDescriptor.sAddressMode = MTLSamplerAddressModeRepeat;
    samplerDescriptor.tAddressMode = MTLSamplerAddressModeRepeat;
    
    samplerState = [device newSamplerStateWithDescriptor:samplerDescriptor];
    
    /*MTLHeapDescriptor *heapDescriptor = [MTLHeapDescriptor new];
     heapDescriptor.storageMode = MTLStorageModePrivate;
     heapDescriptor.cpuCacheMode = MTLCPUCacheModeDefaultCache;
     heapDescriptor.size = 300 * 1024 * 1024; //300mb
     
     heap = [device newHeapWithDescriptor:heapDescriptor];
     
     tempBuffer = [device newBufferWithLength:15 * 1024 * 1024 options:MTLStorageModeShared];
     
     fence = [device newFence];*/
    MTLDepthStencilDescriptor* depthDescriptor = [MTLDepthStencilDescriptor new];
    depthDescriptor.depthCompareFunction = MTLCompareFunctionLess;
    depthDescriptor.depthWriteEnabled = true;
    depthStencilState = [device newDepthStencilStateWithDescriptor:depthDescriptor];
}

void MetalRenderer::createRenderPipelines()
{
    id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[1].offset = 12;
        vertexDescriptor.attributes[1].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[1].bufferIndex = 0;
        vertexDescriptor.attributes[2].offset = 24;
        vertexDescriptor.attributes[2].format = MTLVertexFormatUInt;
        vertexDescriptor.attributes[2].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 28;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 36;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatInvalid;
        
        DefaultPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 52;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        NSError* error;
        DefaultExPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:&error];
        //NSLog([error localizedDescription]);
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 40;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCCTPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 64;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCCTTTTPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 72;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCCNCCTTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCTTTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 60;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCTTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCTTTX_2PipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCTTTX_3PipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 64;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCCTTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 64;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCCTTX_2PipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 72;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCCTTTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 64;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCCNCCTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 60;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCCNCTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 44;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCCNCTPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 48;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCCTTPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 56;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCCTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 24;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCTPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 20;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PTPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 28;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PTTPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 28;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 8;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 16;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 20;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCCPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 32;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PCCH2H4PipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 32;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCH2PipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    {
        MTLRenderPipelineDescriptor* renderDescriptor = [MTLRenderPipelineDescriptor new];
        renderDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
        renderDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
        //
        MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
        vertexDescriptor.attributes[0].offset = 0;
        vertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
        vertexDescriptor.attributes[0].bufferIndex = 0;
        vertexDescriptor.attributes[3].offset = 12;
        vertexDescriptor.attributes[3].format = MTLVertexFormatFloat2;
        vertexDescriptor.attributes[3].bufferIndex = 0;
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 76;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        //renderDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        
        PNCTTTTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
}

/*MTLRenderPipelineDescriptor* createRenderDescriptor(MTLDevicePtr device, std::vector<VertexAttributes>& attributes)
 {
 id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
 
 MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
 descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
 descriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
 descriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
 //
 MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
 
 for (auto attribute : attributes) {
 vertexDescriptor.attributes[attribute.index].offset = attribute.offset;
 vertexDescriptor.attributes[attribute.index].format = attribute.format;
 vertexDescriptor.attributes[attribute.index].bufferIndex = 0;
 }
 //
 vertexDescriptor.layouts[0].stepRate = 1;
 vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
 vertexDescriptor.layouts[0].stride = attributes[0].stride;
 //
 descriptor.vertexDescriptor = vertexDescriptor;
 
 return descriptor;
 }*/

VertexBufferHandle MetalRenderer::createVertexBuffer(uint32_t size, const uint8_t* pointer)
{
    id<MTLBuffer> buffer = [device newBufferWithBytes:pointer length:size options:MTLResourceStorageModeShared];
    /*if (size > 15 * 1024 * 1024)
     printf("ERROR\n");
     //PERFORMANCE!!!
     memcpy([tempBuffer contents], &pointer[0], size);
     
     id<MTLBuffer> buffer = [device newBufferWithLength:size options:MTLResourceStorageModePrivate];
     
     id <MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
     
     id <MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];
     
     [blitEncoder copyFromBuffer:tempBuffer sourceOffset:0 toBuffer:buffer destinationOffset:0 size:size];
     
     [blitEncoder endEncoding];
     
     [commandBuffer commit];
     [commandBuffer waitUntilCompleted];*/
    
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
    // if (size >= 15 * 1024 * 1024)
    //    printf("ERROR\n");
    //PERFORMANCE
    
    //memcpy([tempBuffer contents], &pointer[0], size);
    
    id<MTLBuffer> buffer = [device newBufferWithBytes:pointer length:size options:MTLResourceStorageModeShared];
    
    //id<MTLBuffer> buffer = [heap newBufferWithLength:size options:MTLResourceCPUCacheModeDefaultCache | MTLResourceStorageModePrivate];
    /*id<MTLBuffer> buffer = [device newBufferWithLength:size options:MTLResourceStorageModePrivate];
     
     id <MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
     
     id <MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];
     
     [blitEncoder copyFromBuffer:tempBuffer sourceOffset:0 toBuffer:buffer destinationOffset:0 size:size];
     
     [blitEncoder endEncoding];
     
     [commandBuffer commit];
     [commandBuffer waitUntilCompleted];
     
     
     NSLog(@"HEAP SIZE %lu MB", (unsigned long)[heap usedSize] / 1024 / 1024);
     //id<MTLBuffer> buffer = [device newBufferWithLength:size options:MTLResourceStorageModeShared];
     //memcpy([buffer contents], &pointer[0], size);
     //id<MTLBuffer> buffer = [device newBufferWithBytes:pointer length:size options:MTLResourceStorageModeShared];
     */
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

//#define MACOS_SUPPORT

TextureHandle MetalRenderer::createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format)
{
    bool convert = true;
    MTLPixelFormat textureFormat = MTLPixelFormatInvalid;
    
    switch (format) {
        case D3DFMT_A8R8G8B8:
            textureFormat = MTLPixelFormatR8Uint;
            break;
        case D3DFMT_A1R5G5B5:
            textureFormat = MTLPixelFormatR8Uint;
            break;
        case D3DFMT_A8:
            textureFormat = MTLPixelFormatR8Uint;
            break;
        case D3DFMT_A8B8G8R8:
            textureFormat = MTLPixelFormatR8Uint;
            break;
        case D3DFMT_L8:
            textureFormat = MTLPixelFormatR8Uint;
            break;
#ifdef MACOS_SUPPORT
        case D3DFMT_DXT1:
            textureFormat = MTLPixelFormatBC1_RGBA;
            break;
        case D3DFMT_DXT3:
            textureFormat = MTLPixelFormatBC2_RGBA;
            break;
        case D3DFMT_DXT5:
            textureFormat = MTLPixelFormatBC3_RGBA;
            break;
        case D3DFMT_ATI1:
            textureFormat = MTLPixelFormatBC4_RUnorm;
            break;
        case D3DFMT_ATI2:
            textureFormat = MTLPixelFormatBC4_RUnorm;
            break;
        case D3DFMT_BC7:
            textureFormat = MTLPixelFormatBC7_RGBAUnorm;
            break;
#else
        case D3DFMT_DXT1:
        case D3DFMT_DXT3:
        case D3DFMT_DXT5:
        case D3DFMT_ATI1:
        case D3DFMT_ATI2:
        case D3DFMT_BC7:
            textureFormat = MTLPixelFormatRGBA8Unorm;
            break;
#endif
        default:
            //printf("UNDEFINED TEXTURE FORMAT");
            textureFormat = MTLPixelFormatInvalid;
    }
    
    //if (compatFormat != MTLPixelFormatBC3_RGBA && width < 2048)
    if (format != D3DFMT_DXT5 && format != D3DFMT_DXT1 && format != D3DFMT_DXT3)
        return TextureHandle{0};
    
    if (convert && (width * height * 4) > 20 * 1024 * 1024)
        printf("ERROR IN DECOMPRESSING TEXTURE\n");
    
    MTLTextureDescriptor* descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:textureFormat width:width height:height mipmapped:true];
    
    id <MTLTexture> texture = [device newTextureWithDescriptor:descriptor];
    if (convert)
    {
        for (int i = 0; i < levels; i++) {
            int offset = 0;
            int blockSize = (format == D3DFMT_DXT1) ? 8 : 16;
            
            unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
            
            Decompressor::imageDecodeToBgra8(textureDecompressedMem.get(), pointer + offset, width, height, width * 4, format);
            
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            [texture replaceRegion:region mipmapLevel:i slice:0 withBytes:textureDecompressedMem.get() bytesPerRow:width * 32 / 8 bytesPerImage:0];
            
            offset += size;
            width = std::max(width / 2, 1);
            height = std::max(height / 2, 1);
        }
    } else {
        for (int i = 0; i < levels; i++) {
            int offset = 0;
            int bytesPerBlock = (format == D3DFMT_DXT1) ? 8 : 16;
            int blockWidth = 4;
            
            unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * bytesPerBlock;
            
            int blocksPerRow = (width + (blockWidth - 1)) / blockWidth;
            int bytesPerRow = blocksPerRow * bytesPerBlock;
            
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            [texture replaceRegion:region mipmapLevel:i withBytes:pointer + offset bytesPerRow:bytesPerRow];
            
            offset += size;
            width = std::max(width / 2, 1);
            height = std::max(height / 2, 1);
        }
        
    }
    
    TextureHandle handle;
    
    //find free
    BOOL found = false;
    for (int i = 0; i < gpuBufferSize; i++) {
        if (textures[i] == 0)
        {
            if (testID == 0)
                testID = i;
            textures[i] = texture;
            handle.id = i;
            found = true;
            break;
        }
    }
    
    texturesSize += [textures[handle.id] allocatedSize];
    NSLog(@"%uMB", texturesSize/1024/1024);
    assert(found);
    
    return handle;
    
}

void MetalRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
    assert(vertexBuffers[handle.id] != 0);
    [vertexBuffers[handle.id] setPurgeableState:MTLPurgeableStateEmpty];
    [vertexBuffers[handle.id] release];
    
    vertexBuffers[handle.id] = nil;
}

void MetalRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
    assert(indexBuffers[handle.id] != 0);
    indexBuffers[handle.id] = nil;
}

void MetalRenderer::removeTexture(TextureHandle handle)
{
    texturesSize -= [textures[handle.id] allocatedSize];
    //assert(textures[handle.id] != 0);
    if (handle.id != 0)
        textures[handle.id] = nil;
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
            switch (geometry.type) {
                case Default:
                    [commandEncoder setRenderPipelineState:DefaultPipelineState];
                    break;
                case DefaultEx:
                    [commandEncoder setRenderPipelineState:DefaultExPipelineState];
                    break;
                case PNCCT:
                    [commandEncoder setRenderPipelineState:PNCCTPipelineState];
                    break;
                case PNCCTTTT:
                    [commandEncoder setRenderPipelineState:PNCCTTTTPipelineState];
                    break;
                case PCCNCCTTX:
                    [commandEncoder setRenderPipelineState:PCCNCCTTXPipelineState];
                    break;
                case PCCNCCT:
                    [commandEncoder setRenderPipelineState:PCCNCCTPipelineState];
                    break;
                case PNCTTTX:
                    [commandEncoder setRenderPipelineState:PNCTTTXPipelineState];
                    break;
                case PNCTTX:
                    [commandEncoder setRenderPipelineState:PNCTTXPipelineState];
                    break;
                case PNCTTTX_2:
                    [commandEncoder setRenderPipelineState:PNCTTTX_2PipelineState];
                    break;
                case PNCTTTX_3:
                    [commandEncoder setRenderPipelineState:PNCTTTX_3PipelineState];
                    break;
                case PNCCTTX:
                    [commandEncoder setRenderPipelineState:PNCCTTXPipelineState];
                    break;
                case PNCCTTX_2:
                    [commandEncoder setRenderPipelineState:PNCCTTX_2PipelineState];
                    break;
                case PNCCTTTX:
                    [commandEncoder setRenderPipelineState:PNCCTTTXPipelineState];
                    break;
                case PCCNCCTX:
                    [commandEncoder setRenderPipelineState:PCCNCCTXPipelineState];
                    break;
                case PCCNCTX:
                    [commandEncoder setRenderPipelineState:PCCNCTXPipelineState];
                    break;
                case PCCNCT:
                    [commandEncoder setRenderPipelineState:PCCNCTPipelineState];
                    break;
                case PNCCTT:
                    [commandEncoder setRenderPipelineState:PNCCTTPipelineState];
                    break;
                case PNCCTX:
                    [commandEncoder setRenderPipelineState:PNCCTXPipelineState];
                    break;
                case PCT:
                    [commandEncoder setRenderPipelineState:PCTPipelineState];
                    break;
                case PT:
                    [commandEncoder setRenderPipelineState:PTPipelineState];
                    break;
                case PTT:
                    [commandEncoder setRenderPipelineState:PTTPipelineState];
                    break;
                case PNC:
                    [commandEncoder setRenderPipelineState:PNCPipelineState];
                    break;
                case PC:
                    [commandEncoder setRenderPipelineState:PCPipelineState];
                    break;
                case PCC:
                    [commandEncoder setRenderPipelineState:PCCPipelineState];
                    break;
                case PCCH2H4:
                    [commandEncoder setRenderPipelineState:PCCH2H4PipelineState];
                    break;
                case PNCH2:
                    [commandEncoder setRenderPipelineState:PNCH2PipelineState];
                    break;
                case PNCTTTTX:
                    [commandEncoder setRenderPipelineState:PNCTTTTXPipelineState];
                    break;
                default:
                    continue;
            }
            //
            //[commandEncoder setDepthStencilState:depthStencilState];
            [commandEncoder setCullMode:MTLCullModeBack];
            //if (geometry.type != VertexType::Default)
            // continue;
            //[commandEncoder setRenderPipelineState:DefaultPipelineState];
            //[commandEncoder setVertexBuffer:cubeTestBuffer offset:0 atIndex:0];
            //[commandEncoder setVertexBuffer:vertexBuffers[0] offset:0 atIndex:0];
            if (testID != 0) {
                [commandEncoder setFragmentTexture:textures[testID] atIndex:0];
            }
            
            [commandEncoder setFragmentSamplerState:samplerState atIndex:0];
            
            [commandEncoder setVertexBuffer:vertexBuffers[geometry.getVertexBufferHandle().id] offset:0 atIndex:0];
            //memcpy([scene_buffer contents], &scene_matrices, sizeof(uniform_buffer_struct));
            
            //[commandEncoder setVertexBuffer:scene_buffer offset:0 atIndex:1];
            
            //[commandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:36];
            
            [commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:geometry.getIndexCount() indexType:MTLIndexTypeUInt16 indexBuffer:indexBuffers[geometry.getIndexBufferHandle().id] indexBufferOffset:0];
            
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
        //fix
        if (ydr)
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
    
    //NSLog(@"Player Pos%f %f %f", playerPos.x, playerPos.y, playerPos.z);
    //NSLog(@"%f %f %f", curCamera->getPosition().x, curCamera->getPosition().y, curCamera->getPosition().z);
    //curCamera->setPosition(curCamera->getPosition() + glm::vec3(0,0, -1));
    
    glm::mat4 projection = curCamera->getProjection();
    glm::mat4 view = curCamera->getViewMatrix();
    glm::mat4 projectionView = projection * view;
    //
    curCamera->updateFrustum(projectionView); //maybe remove this??? opengl ONLY
    
    id <CAMetalDrawable> drawable = [mtkView currentDrawable];
    
    mainPassDescriptor.colorAttachments[0].texture = drawable.texture;
    
    id <MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:mainPassDescriptor];
    //
    scene_matrices.projViewMatrix = toMtl(projectionView);
    
    for (auto& object : world->renderList)
    {
        scene_matrices.modelMatrix = toMtl(object->getMatrix());
        [commandEncoder setVertexBytes:&scene_matrices length:sizeof(uniform_buffer_struct) atIndex:1];
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
    }
    
    [commandEncoder endEncoding];
    
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}
