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

#include <simd/simd.h>
#include "glm/glm.hpp"
//id <MTLDevice> device;
//MTKView* metalView;

struct VertexAttributes
{
    int index;
    MTLVertexFormat format;
    int stride;
    int offset;
};

const std::vector<VertexAttributes> DefaultAttrib = {
    {0, MTLVertexFormatFloat3, 36, 0},
};
/*const std::vector<VertexAttributes> DefaultExAttrib = {
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
 {3, 2, GL_HALF_FLOAT, 32, (GLvoid*)20},  //    FIXED
 };
 const std::vector<VertexAttributes> PNCH2Attrib = {
 {0, 3, GL_FLOAT, 32, nullptr},
 {1, 3, GL_FLOAT, 32, (GLvoid*)12},
 {2, 4, GL_UNSIGNED_BYTE, 32, (GLvoid*)24},
 {3, 2, GL_HALF_FLOAT, 32, (GLvoid*)28}  //    FIXED
 };
 const std::vector<VertexAttributes> PNCTTTTXAttrib = {
 {0, 3, GL_FLOAT, 76, nullptr}, {1, 3, GL_FLOAT, 76, (GLvoid*)12}, {2, 4, GL_UNSIGNED_BYTE, 76, (GLvoid*)24}, {3, 2, GL_FLOAT, 76, (GLvoid*)28}};
 const std::vector<VertexAttributes> PNCTTTTAttrib = {
 };
 const std::vector<VertexAttributes> PCCNCCTTAttrib = {
 };
 const std::vector<VertexAttributes> PCTTAttrib = {
 };
 const std::vector<VertexAttributes> PCCCCTAttrib = {
 };
 const std::vector<VertexAttributes> PCCNCAttrib = {
 };*/
/*const std::vector<VertexAttributes> PCCNCTTAttrib = {
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
 
 */
#define gpuBufferSize 16000
id<MTLBuffer> vertexBuffers[gpuBufferSize] = {0};
id<MTLBuffer> indexBuffers[gpuBufferSize] = {0};
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
//id<MTLRenderPipelineState> PTTPipelineState;
//
id <MTLBuffer> scene_buffer;

id <MTLBuffer> cubeTestBuffer;

id <MTLRenderCommandEncoder> commandEncoder;

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
    device = MTLCreateSystemDefaultDevice();
    commandQueue = [device newCommandQueue];
    NSLog(@"METAL RENDERER IS READY!");
    
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 36;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 52;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
        DefaultExPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 40;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 64;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 72;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 60;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 64;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 68;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 72;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 64;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 60;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 44;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 48;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 56;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 24;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 20;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 28;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 28;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 16;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 20;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 32;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 32;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
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
        //
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stride = 76;
        //
        renderDescriptor.vertexDescriptor = vertexDescriptor;
        
        PNCTTTTXPipelineState = [device newRenderPipelineStateWithDescriptor:renderDescriptor error:nil];
    }
    
    //
    
    mainPassDescriptor = [MTLRenderPassDescriptor new];
    mainPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    mainPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 104/255, 55/255, 1.0);
    mainPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    
    scene_buffer = [device newBufferWithLength:sizeof(uniform_buffer_struct) options:MTLResourceStorageModeShared];
    
    cubeTestBuffer = [device newBufferWithBytes:&cubeVertices[0] length:sizeof(cubeVertices)  options:MTLResourceStorageModeShared];
}

MTLRenderPipelineDescriptor* createRenderDescriptor(MTLDevicePtr device, std::vector<VertexAttributes>& attributes)
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
    [vertexBuffers[handle.id] setPurgeableState:MTLPurgeableStateEmpty];
    [vertexBuffers[handle.id] release];
    
    vertexBuffers[handle.id] = nil;
}

void MetalRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
    assert(indexBuffers[handle.id] != 0);
    indexBuffers[handle.id] = nil;
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
                /*case PNCCTTX_2:
                    [commandEncoder setRenderPipelineState:PNCCTTX_2PipelineState];
                    break;*/
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
            
            //if (geometry.type != VertexType::Default)
               // continue;
            //[commandEncoder setRenderPipelineState:DefaultPipelineState];
            //[commandEncoder setVertexBuffer:cubeTestBuffer offset:0 atIndex:0];
            //[commandEncoder setVertexBuffer:vertexBuffers[0] offset:0 atIndex:0];
            [commandEncoder setVertexBuffer:vertexBuffers[geometry.getVertexBufferHandle().id] offset:0 atIndex:0];
            [commandEncoder setVertexBytes:&scene_matrices length:sizeof(uniform_buffer_struct) atIndex:1];
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
