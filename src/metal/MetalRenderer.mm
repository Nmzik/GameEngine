//
//  MetalRenderer.cpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#include "MetalRenderer.h"

#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"
#include "../loaders/YdrLoader.h"
#include "TextureDecompressor.h"

#include <simd/simd.h>

uint32_t texturesSize = 0;
uint32_t vertexBufferSize = 0;
static int testID = 0;

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
id <MTLBuffer> scene_buffer;
id <MTLRenderCommandEncoder> commandEncoder;
id <MTLDepthStencilState> depthStencilState;
id <MTLTexture> depthTexture;
id <MTLTexture> errorTexture;
//id <MTLHeap> heap;

//id<MTLBuffer> tempBuffer;

//id <MTLFence> fence;

static const MTLVertexFormat attribType[3][4] =
{
    {
        MTLVertexFormatUChar,
        MTLVertexFormatUChar2,
        MTLVertexFormatUChar3,
        MTLVertexFormatUChar4,
    },
    {
        MTLVertexFormatHalf,
        MTLVertexFormatHalf2,
        MTLVertexFormatHalf3,
        MTLVertexFormatHalf4,
    },
    {
        MTLVertexFormatFloat,
        MTLVertexFormatFloat2,
        MTLVertexFormatFloat3,
        MTLVertexFormatFloat4,
    },
};


struct uniform_buffer_struct {
    matrix_float4x4 projMatrix;
    matrix_float4x4 ViewMatrix;
    matrix_float4x4 modelMatrix;
} scene_matrices;

MetalRenderer::MetalRenderer()
{
    textureDecompressedMem = std::make_unique<uint8_t[]>(20 * 1024 * 1024); //20mb for uncompressing textures
}

MetalRenderer::~MetalRenderer()
{
    
}

void MetalRenderer::initializeRenderEngine()
{
    srand(time(NULL));
    commandQueue = [device newCommandQueue];
    
    createRenderPipelines();
    //
    mainPassDescriptor = [MTLRenderPassDescriptor new];
    mainPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    mainPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
    mainPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    mainPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    mainPassDescriptor.depthAttachment.clearDepth = 1.0f;
    
    scene_buffer = [device newBufferWithLength:sizeof(uniform_buffer_struct) options:MTLResourceStorageModeShared];
    
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
    
#if TARGET_OS_IPHONE
    CGSize depthTextureSize = CGSizeMake(mtkView.frame.size.width * mtkView.contentScaleFactor, mtkView.frame.size.height * mtkView.contentScaleFactor);
#else
    CGSize depthTextureSize = CGSizeMake(mtkView.frame.size.width, mtkView.frame.size.height);
#endif
    
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float width:depthTextureSize.width height:depthTextureSize.height mipmapped:NO];
    desc.storageMode = MTLStorageModePrivate;
    desc.usage = MTLTextureUsageRenderTarget;
    
    depthTexture = [device newTextureWithDescriptor:desc];
    
    createWarningTexture();
}

void MetalRenderer::createWarningTexture()
{
    MTLTextureDescriptor* texDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:8 height:8 mipmapped:false];
    
    errorTexture = [device newTextureWithDescriptor:texDescriptor];
    
    uint8_t data[8 * 8 * 4];
    for (int i=0;i<8*8;++i)
    {
        data[4*i] = (255);
        data[4*i+1] = (0);
        data[4*i+2] = (255);
        data[4*i+3] = (1);
    }
    
    MTLRegion region = MTLRegionMake2D(0, 0, 8, 8);
    [errorTexture replaceRegion:region mipmapLevel:0 withBytes:&data[0] bytesPerRow:8 * 4];
    
    textures[0] = errorTexture;
}

void MetalRenderer::createRenderPipelines()
{
    //id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
    
    DefaultPipelineState = createRenderDescriptor(DefaultAttrib);
    DefaultExPipelineState = createRenderDescriptor(DefaultExAttrib);
    PNCCTPipelineState = createRenderDescriptor(PNCCTAttrib);
    PNCCTTTTPipelineState = createRenderDescriptor(PNCCTTTTAttrib);
    PCCNCCTTXPipelineState = createRenderDescriptor(PCCNCCTTXAttrib);
    PNCTTTXPipelineState = createRenderDescriptor(PNCTTTXAttrib);
    PNCTTXPipelineState = createRenderDescriptor(PNCTTXAttrib);
    PNCTTTX_2PipelineState = createRenderDescriptor(PNCTTTX_2Attrib);
    PNCTTTX_3PipelineState = createRenderDescriptor(PNCTTTX_3Attrib);
    PNCCTTXPipelineState = createRenderDescriptor(PNCCTTXAttrib);
    PNCCTTX_2PipelineState = createRenderDescriptor(PNCCTTX_2Attrib);
    PNCCTTTXPipelineState = createRenderDescriptor(PNCCTTTXAttrib);
    PCCNCCTXPipelineState = createRenderDescriptor(PCCNCCTXAttrib);
    PCCNCTXPipelineState = createRenderDescriptor(PCCNCTXAttrib);
    PCCNCTPipelineState = createRenderDescriptor(PCCNCTAttrib);
    PNCCTTPipelineState = createRenderDescriptor(PNCCTTAttrib);
    PNCCTXPipelineState = createRenderDescriptor(PNCCTXAttrib);
    PCTPipelineState = createRenderDescriptor(PCTAttrib);
    PTPipelineState = createRenderDescriptor(PTAttrib);
    PTTPipelineState = createRenderDescriptor(PTTAttrib);
    PNCPipelineState = createRenderDescriptor(PNCAttrib);
    PCPipelineState = createRenderDescriptor(PCAttrib);
    PCCPipelineState = createRenderDescriptor(PCCAttrib);
    PCCH2H4PipelineState = createRenderDescriptor(PCCH2H4Attrib);
    PNCH2PipelineState = createRenderDescriptor(PNCH2Attrib);
    PNCTTTTXPipelineState = createRenderDescriptor(PNCTTTTXAttrib);
}

MTLRenderPipelineState MetalRenderer::createRenderDescriptor(VertexLayout& attributes)
{
    id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
    
    MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
    descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    descriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"basic_vertex"];
    descriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"basic_fragment"];
    //
    MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];
    
    for (int i = 0; i < attributes.size; i++) {
        int index = attributes.attributes[i].index;
        vertexDescriptor.attributes[index].offset = attributes.attributes[i].offset;
        vertexDescriptor.attributes[index].format = attribType[attributes.attributes[i].type][attributes.attributes[i].size - 1];
        vertexDescriptor.attributes[index].bufferIndex = 0;
    }
    //
    vertexDescriptor.layouts[0].stepRate = 1;
    vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
    vertexDescriptor.layouts[0].stride = attributes.stride;
    //
    descriptor.vertexDescriptor = vertexDescriptor;
    descriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
    
    id <MTLRenderPipelineState> pipelineState = [device newRenderPipelineStateWithDescriptor:descriptor error:nil];
    return pipelineState;
}

VertexBufferHandle MetalRenderer::createVertexBuffer(uint32_t size, const uint8_t* pointer)
{
    id<MTLBuffer> buffer = [device newBufferWithBytes:pointer length:size options:MTLResourceStorageModeShared];
    vertexBufferSize++;
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

#include <time.h>

IndexBufferHandle MetalRenderer::createIndexBuffer(uint32_t size, const uint8_t* pointer)
{
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

TextureHandle MetalRenderer::createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format)
{
    bool compressed = true;
#if TARGET_OS_IPHONE
    bool convert = true;
#else
    bool convert = false;
#endif
    MTLPixelFormat textureFormat = MTLPixelFormatInvalid;
    
    switch (format) {
        case D3DFMT_A8R8G8B8:
        textureFormat = MTLPixelFormatRGBA8Unorm;
        compressed = false;
        break;
        case D3DFMT_A1R5G5B5:
        //textureFormat = MTLPixelFormat(41);
        textureFormat = MTLPixelFormatInvalid;
        compressed = false;
        break;
        case D3DFMT_A8:
        textureFormat = MTLPixelFormatA8Unorm;
        compressed = false;
        break;
        case D3DFMT_A8B8G8R8:
        textureFormat = MTLPixelFormatRGBA8Unorm;
        compressed = false;
        break;
        case D3DFMT_L8:
        textureFormat = MTLPixelFormatR8Uint;
        compressed = false;
        break;
#if TARGET_OS_IPHONE
        case D3DFMT_DXT1:
        case D3DFMT_DXT3:
        case D3DFMT_DXT5:
        case D3DFMT_ATI1:
        case D3DFMT_ATI2:
        case D3DFMT_BC7:
        textureFormat = MTLPixelFormatRGBA8Unorm;
        break;
#else
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
#endif
        default:
        //printf("UNDEFINED TEXTURE FORMAT");
        textureFormat = MTLPixelFormatInvalid;
    }
    
    //if (compatFormat != MTLPixelFormatBC3_RGBA && width < 2048)
    if (textureFormat == MTLPixelFormatInvalid)
        return TextureHandle{0};
    
    //check uncompressed texture size
#if TARGET_OS_IPHONE
    assert(convert && (width * height * 4) <= 20 * 1024 * 1024);
#endif
    
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
    } else if (!compressed){
        for (int i = 0; i < levels; i++) {
            
            int offset = 0;
            
            unsigned int size =
            ((width + 1) >> 1) * ((height + 1) >> 1) * 4;
            
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            [texture replaceRegion:region mipmapLevel:i withBytes:pointer + offset bytesPerRow:width * 4];
            
            offset += size;
            width = std::max(width / 2, 1);
            height = std::max(height / 2, 1);
        }
    }
    #if !TARGET_OS_IPHONE
    else {
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
#endif
    
    TextureHandle handle;
    
    //find free
    BOOL found = false;
    for (int i = 0; i < gpuBufferSize; i++) {
        if (textures[i] == 0)
        {
            //if (format == D3DFMT_A8R8G8B8 && testID == 0)
             //   testID = i;
            //}
            textures[i] = texture;
            handle.id = i;
            found = true;
            break;
        }
    }
    
    texturesSize += [textures[handle.id] allocatedSize];
    //NSLog(@"%uMB", texturesSize/1024/1024);
    assert(found);
    
    return handle;
    
}

void MetalRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
    vertexBufferSize--;
    assert(vertexBuffers[handle.id] != 0);
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
    {
        textures[handle.id] = nil;
    }
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
                /*(case PCT:
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
                 break;*/
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
            [commandEncoder setDepthStencilState:depthStencilState];
            [commandEncoder setCullMode:MTLCullModeBack];
            //if (geometry.type != VertexType::Default)
            // continue;
            //if (testID != 0)
            //[commandEncoder setFragmentTexture:textures[testID] atIndex:0];
            //else
            
            //fix TextureManager
            if (textures[geometry.getTextureHandle().id] == 0)
                [commandEncoder setFragmentTexture:textures[0] atIndex:0];
            else
                [commandEncoder setFragmentTexture:textures[geometry.getTextureHandle().id] atIndex:0];
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
    mainPassDescriptor.depthAttachment.texture = depthTexture;
    
    id <MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:mainPassDescriptor];
    //
    scene_matrices.projMatrix = toMtl(projection);
    scene_matrices.ViewMatrix = toMtl(view);
    
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
