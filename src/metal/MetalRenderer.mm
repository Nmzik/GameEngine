//
//  MetalRenderer.cpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "MetalRenderer.h"

#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"
#include "../loaders/YdrLoader.h"
#include "TextureDecompressor.h"

#include <simd/simd.h>

uint32_t texturesSize = 0;
uint32_t vertexBufferSize = 0;

id<MTLBuffer> vertexBuffers[gpuBufferSize] = {0};
id<MTLBuffer> indexBuffers[gpuBufferSize] = {0};
id<MTLTexture> textures[gpuBufferSize] = {0};

std::stack<uint32_t> vertexBufferIDs;
std::stack<uint32_t> indexBufferIDs;
std::stack<uint32_t> texturesIDs;

id<MTLCommandQueue> commandQueue;
MTLRenderPassDescriptor* mainPassDescriptor;
MTLRenderPassDescriptor* guiPassDescriptor;
MTLRenderPassDescriptor* fxaaPassDescriptor;
//
id<MTLRenderPipelineState> guiPipelineState;
id<MTLRenderPipelineState> fxaaPipelineState;
id<MTLRenderPipelineState> DefaultPipelineState;
id<MTLRenderPipelineState> DefaultExPipelineState;
id<MTLRenderPipelineState> PNCCTPipelineState;
id<MTLRenderPipelineState> PNCCTTTTPipelineState;
id<MTLRenderPipelineState> PBBNCCTTXPipelineState;
id<MTLRenderPipelineState> PBBNCCTPipelineState;
id<MTLRenderPipelineState> PNCTTTXPipelineState;
id<MTLRenderPipelineState> PNCTTXPipelineState;
id<MTLRenderPipelineState> PNCTTTX_2PipelineState;
id<MTLRenderPipelineState> PNCTTTX_3PipelineState;
id<MTLRenderPipelineState> PNCCTTXPipelineState;
id<MTLRenderPipelineState> PNCCTTX_2PipelineState;
id<MTLRenderPipelineState> PNCCTTTXPipelineState;
id<MTLRenderPipelineState> PBBNCCTXPipelineState;
id<MTLRenderPipelineState> PBBNCTXPipelineState;
id<MTLRenderPipelineState> PBBNCTPipelineState;
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
id <MTLRenderCommandEncoder> guiEncoder;

id <MTLDepthStencilState> depthStencilState;
id <MTLTexture> mainTexture;
id <MTLTexture> depthTexture;
id <MTLTexture> errorTexture;
id <MTLTexture> fontTexture;
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
    for (int i = 0; i < gpuBufferSize; i++) {
        vertexBufferIDs.push(i);
        indexBufferIDs.push(i);
        texturesIDs.push(i);
    }
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
    createDepthTexture();
    createWarningTexture();
    //createFontTexture();
    //
#if TARGET_OS_IPHONE
    CGSize textureSize = CGSizeMake(mtkView.frame.size.width * mtkView.contentScaleFactor, mtkView.frame.size.height * mtkView.contentScaleFactor);
#else
    CGSize textureSize = CGSizeMake(mtkView.frame.size.width, mtkView.frame.size.height);
#endif
    
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:textureSize.width height:textureSize.height mipmapped:NO];
    desc.storageMode = MTLStorageModePrivate;
    desc.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
    
    mainTexture = [device newTextureWithDescriptor:desc];
    
    //
    mainPassDescriptor = [MTLRenderPassDescriptor new];
    mainPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    mainPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
    mainPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    mainPassDescriptor.colorAttachments[0].texture = mainTexture;
    mainPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    mainPassDescriptor.depthAttachment.clearDepth = 1.0f;
    mainPassDescriptor.depthAttachment.texture = depthTexture;
    
    /*{
    guiPassDescriptor = [MTLRenderPassDescriptor new];
    guiPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionLoad;
    guiPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
     
        MTLRenderPipelineDescriptor* guiDescriptor = [MTLRenderPipelineDescriptor new];
        guiDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        guiDescriptor.colorAttachments[0].blendingEnabled = true;
        guiDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        guiDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        
        id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
        guiDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertex_gui"];
        guiDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragment_gui"];
        
        NSError* error;
        guiPipelineState = [device newRenderPipelineStateWithDescriptor:guiDescriptor error:&error];
        NSLog(@"%@", [error debugDescription]);
    }*/
    
    /*{
        fxaaPassDescriptor = [MTLRenderPassDescriptor new];
        fxaaPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionLoad;
        fxaaPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionDontCare;
    }
    {
        MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
        descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        
        id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
        descriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"fxaa_vertex"];
        descriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fxaa_fragment"];
        
        NSError* error;
        fxaaPipelineState = [device newRenderPipelineStateWithDescriptor:descriptor error:&error];
        NSLog(@"%@", [error debugDescription]);
    }*/
    
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
}

void MetalRenderer::createDepthTexture()
{
#if TARGET_OS_IPHONE
    CGSize depthTextureSize = CGSizeMake(mtkView.frame.size.width * mtkView.contentScaleFactor, mtkView.frame.size.height * mtkView.contentScaleFactor);
#else
    CGSize depthTextureSize = CGSizeMake(mtkView.frame.size.width, mtkView.frame.size.height);
#endif
    
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float width:depthTextureSize.width height:depthTextureSize.height mipmapped:NO];
    desc.storageMode = MTLStorageModePrivate;
    desc.usage = MTLTextureUsageRenderTarget;
    
    depthTexture = [device newTextureWithDescriptor:desc];
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

void MetalRenderer::createFontTexture()
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image = stbi_load("/Users/nmzik/Desktop/lucidagrande.jpg",
                                     &width,
                                     &height,
                                     &channels,
                                     STBI_rgb_alpha);
    
    
    MTLTextureDescriptor* texDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:width height:height mipmapped:false];
    
    fontTexture = [device newTextureWithDescriptor:texDescriptor];
    MTLRegion region = MTLRegionMake2D(0, 0, width, height);
    [fontTexture replaceRegion:region mipmapLevel:0 withBytes:&image[0] bytesPerRow:width * 4];
}

void MetalRenderer::createRenderPipelines()
{
    //id<MTLLibrary> defaultLibrary = [device newDefaultLibrary];
    
    DefaultPipelineState = createRenderDescriptor(Default_Attrib);
    DefaultExPipelineState = createRenderDescriptor(DefaultEx_Attrib);
    PNCCTPipelineState = createRenderDescriptor(PNCCT_Attrib);
    PNCCTTTTPipelineState = createRenderDescriptor(PNCCTTTT_Attrib);
    PBBNCCTTXPipelineState = createRenderDescriptor(PBBNCCTTX_Attrib);
    PNCTTTXPipelineState = createRenderDescriptor(PNCTTTX_Attrib);
    PNCTTXPipelineState = createRenderDescriptor(PNCTTX_Attrib);
    PNCTTTX_2PipelineState = createRenderDescriptor(PNCTTTX_2_Attrib);
    PNCTTTX_3PipelineState = createRenderDescriptor(PNCTTTX_3_Attrib);
    PNCCTTXPipelineState = createRenderDescriptor(PNCCTTX_Attrib);
    PNCCTTX_2PipelineState = createRenderDescriptor(PNCCTTX_2_Attrib);
    PNCCTTTXPipelineState = createRenderDescriptor(PNCCTTTX_Attrib);
    PBBNCCTXPipelineState = createRenderDescriptor(PBBNCCTX_Attrib);
    PBBNCTXPipelineState = createRenderDescriptor(PBBNCTX_Attrib);
    PBBNCTPipelineState = createRenderDescriptor(PBBNCT_Attrib);
    PNCCTTPipelineState = createRenderDescriptor(PNCCTT_Attrib);
    PNCCTXPipelineState = createRenderDescriptor(PNCCTX_Attrib);
    PCTPipelineState = createRenderDescriptor(PCT_Attrib);
    PTPipelineState = createRenderDescriptor(PT_Attrib);
    PTTPipelineState = createRenderDescriptor(PTT_Attrib);
    PNCPipelineState = createRenderDescriptor(PNC_Attrib);
    PCPipelineState = createRenderDescriptor(PC_Attrib);
    PCCPipelineState = createRenderDescriptor(PCC_Attrib);
    PCCH2H4PipelineState = createRenderDescriptor(PCCH2H4_Attrib);
    PNCH2PipelineState = createRenderDescriptor(PNCH2_Attrib);
    PNCTTTTXPipelineState = createRenderDescriptor(PNCTTTTX_Attrib);
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
        vertexDescriptor.attributes[index].format = attribType[(int)attributes.attributes[i].type][attributes.attributes[i].size - 1];
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
    assert(!vertexBufferIDs.empty());
    uint32_t index = vertexBufferIDs.top();
    vertexBufferIDs.pop();
    
    handle.id = index;
    vertexBuffers[index] = buffer;
    
    return handle;
}

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
    assert(!indexBufferIDs.empty());
    uint32_t index = indexBufferIDs.top();
    indexBufferIDs.pop();
    
    handle.id = index;
    indexBuffers[index] = buffer;
    
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
            textureFormat = MTLPixelFormatBC5_RGUnorm;
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
    
    if (compressed || convert) {
        int minMipMap = 3; //min mipmap to load
        minMipMap = levels > minMipMap? minMipMap : levels - 1;
        
        int blockSize = (format == D3DFMT_DXT1) ? 8 : 16;
        for (int i = 0; i < minMipMap; i++) {
            unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
            pointer += size;
            width = std::max(width / 2, 1);
            height = std::max(height / 2, 1);
        }
        levels -= minMipMap;
    }
    
    MTLTextureDescriptor* descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:textureFormat width:width height:height mipmapped:levels > 1? true : false];
    
    id <MTLTexture> texture = [device newTextureWithDescriptor:descriptor];
    if (convert)
    {
        for (int i = 0; i < levels; i++) {
            int blockSize = (format == D3DFMT_DXT1) ? 8 : 16;
            
            unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
            
            Decompressor::imageDecodeToBgra8(textureDecompressedMem.get(), pointer, width, height, width * 4, format);
            
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            [texture replaceRegion:region mipmapLevel:i slice:0 withBytes:textureDecompressedMem.get() bytesPerRow:width * 32 / 8 bytesPerImage:0];
            
            pointer += size;
            width = std::max(width / 2, 1);
            height = std::max(height / 2, 1);
        }
    } else if (!compressed){
        for (int i = 0; i < levels; i++) {
            unsigned int size =
            ((width + 1) >> 1) * ((height + 1) >> 1) * 4;
            
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            [texture replaceRegion:region mipmapLevel:i withBytes:pointer bytesPerRow:width * 4];
            
            pointer += size;
            width = std::max(width / 2, 1);
            height = std::max(height / 2, 1);
        }
    }
    #if !TARGET_OS_IPHONE
    else {
        for (int i = 0; i < levels; i++) {
            int bytesPerBlock = (format == D3DFMT_DXT1) ? 8 : 16;
            int blockWidth = 4;
            
            unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * bytesPerBlock;
            
            int blocksPerRow = (width + (blockWidth - 1)) / blockWidth;
            int bytesPerRow = blocksPerRow * bytesPerBlock;
            
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            [texture replaceRegion:region mipmapLevel:i withBytes:pointer bytesPerRow:bytesPerRow];
            
            pointer += size;
            width = std::max(width / 2, 1);
            height = std::max(height / 2, 1);
        }
    }
#endif
    
    TextureHandle handle;
    
    //find free
    assert(!texturesIDs.empty());
    uint32_t index = texturesIDs.top();
    texturesIDs.pop();
    
    handle.id = index;
    textures[index] = texture;
    
    return handle;
    
}

void MetalRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
    vertexBufferSize--;
    assert(vertexBuffers[handle.id] != 0);
    vertexBuffers[handle.id] = nil;
    
    vertexBufferIDs.push(handle.id);
}

void MetalRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
    assert(indexBuffers[handle.id] != 0);
    indexBuffers[handle.id] = nil;
    
    indexBufferIDs.push(handle.id);
}

void MetalRenderer::removeTexture(TextureHandle handle)
{
    texturesSize -= [textures[handle.id] allocatedSize];
    //assert(textures[handle.id] != 0);
    if (handle.id != 0)
    {
        textures[handle.id] = nil;
    }
    
    texturesIDs.push(handle.id);
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
                case PBBNCCTTX:
                [commandEncoder setRenderPipelineState:PBBNCCTTXPipelineState];
                break;
                case PBBNCCT:
                [commandEncoder setRenderPipelineState:PBBNCCTPipelineState];
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
                case PBBNCCTX:
                [commandEncoder setRenderPipelineState:PBBNCCTXPipelineState];
                break;
                case PBBNCTX:
                [commandEncoder setRenderPipelineState:PBBNCTXPipelineState];
                break;
                case PBBNCT:
                [commandEncoder setRenderPipelineState:PBBNCTPipelineState];
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
    
    /*guiPassDescriptor.colorAttachments[0].texture = drawable.texture;
    
    guiEncoder = [commandBuffer renderCommandEncoderWithDescriptor:guiPassDescriptor];
    [guiEncoder pushDebugGroup:@"GUI"];
    
    char test[100];
    sprintf(test, "Player: %f %f %f", playerPos.x, playerPos.y, playerPos.z);
    std::string textString(test);
    renderText(textString, glm::vec2(-1.f, -1.0f));
    
    [guiEncoder popDebugGroup];
    [guiEncoder endEncoding];*/
    
    /*id <CAMetalDrawable> drawable = [mtkView currentDrawable];
    fxaaPassDescriptor.colorAttachments[0].texture = drawable.texture;
    
    id <MTLRenderCommandEncoder> fxaaEncoder = [commandBuffer renderCommandEncoderWithDescriptor:fxaaPassDescriptor];
    
    struct guiVertices {
        glm::vec2 vertices;
        glm::vec2 uvs;
    };
    
    std::vector<guiVertices> buffer;
    buffer.push_back({glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)});
    buffer.push_back({glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
                        buffer.push_back({glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
                        buffer.push_back({glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)});
                        buffer.push_back({glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
                        buffer.push_back({glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)});
    
    [fxaaEncoder setRenderPipelineState:fxaaPipelineState];
    [fxaaEncoder setVertexBytes:&buffer[0] length:buffer.size() * sizeof(guiVertices) atIndex:0];
    [fxaaEncoder setFragmentSamplerState:samplerState atIndex:0];
    [fxaaEncoder setFragmentTexture:mainTexture atIndex:0];
    [fxaaEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:buffer.size()];
    
    [fxaaEncoder endEncoding];*/
    
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}

void MetalRenderer::renderText(std::string& text, glm::vec2 pos)
{
    struct guiVertices {
        glm::vec2 vertices;
        glm::vec2 uvs;
    };
    
    std::vector<guiVertices> buffer;
    float x = 0;
    float y = 0;
    
    /*buffer.push_back({glm::vec2(0.5, 0.5), glm::vec2(1.0, 1.0)});
    buffer.push_back({glm::vec2(0, 0.5), glm::vec2(1.0, 1.0)});
    buffer.push_back({glm::vec2(0, 0), glm::vec2(1.0, 1.0)});
    
    buffer.push_back({glm::vec2(0, 0), glm::vec2(1.0, 1.0)});
    buffer.push_back({glm::vec2(0.5, 0), glm::vec2(1.0, 1.0)});
    buffer.push_back({glm::vec2(0.5, 0.5), glm::vec2(1.0, 1.0)});*/
    
    for (int i = 0; i < text.size(); i++) {
        
        char test = text[i];
        
        int startIndex = (int)test - 32;
        
        
        
        int posX = startIndex / 10;
        int posY = startIndex % 10;
        
        //printf("CHAR %c %d %d\n", test, posX, posY);
        
        float uv_x = (startIndex%10)/10.0f;
        float uv_y = (startIndex/10)/10.0f;
        
        
        glm::vec2 pos(30.f, 30.f);
        float resX = 1.0f/pos.x;
        float resY = 1.0f/pos.y;
        
        buffer.push_back({glm::vec2(resX + x, resY), glm::vec2(uv_x+1.0f/10.0f, 1.0f - uv_y)});
        buffer.push_back({glm::vec2(0 + x, resY), glm::vec2(uv_x           , 1.0f - uv_y)});
        buffer.push_back({glm::vec2(0 + x, 0), glm::vec2(uv_x           , 1.0f - (uv_y + 1.0f/10.0f))});
        
        buffer.push_back({glm::vec2(0 + x, 0), glm::vec2(uv_x           , 1.0f - (uv_y + 1.0f/10.0f))});
        buffer.push_back({glm::vec2(resX + x, 0), glm::vec2(uv_x+1.0f/10.0f, 1.0f - (uv_y + 1.0f/10.0f))});
        buffer.push_back({glm::vec2(resX + x, resY), glm::vec2(uv_x+1.0f/10.0f, 1.0f - uv_y)});
        
        x += resX;
    }
    
    glm::vec2 position(-1.f, 0.5f);
    
    [guiEncoder setRenderPipelineState:guiPipelineState];
    [guiEncoder setVertexBytes:&buffer[0] length:buffer.size() * sizeof(guiVertices) atIndex:0];
    [guiEncoder setVertexBytes:&position length:sizeof(glm::vec2) atIndex:1];
    [guiEncoder setFragmentSamplerState:samplerState atIndex:0];
    [guiEncoder setFragmentTexture:fontTexture atIndex:0];
    [guiEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:buffer.size()];
}
