//
//  MetalRenderer.hpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#pragma once

#include <stdio.h>
#include <memory>
#include "../BaseRenderer.h"
#include "../Texture.h"
#include <chrono>
#if defined(__OBJC__)
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
typedef id<MTLDevice> MTLDevicePtr;
typedef MTKView* MTLViewPtr;
typedef id<MTLRenderPipelineState> MTLRenderPipelineState;
#else
#include <objc/NSObjCRuntime.h>
#include <objc/objc.h>
typedef id MTLDevicePtr;
typedef id MTLViewPtr;
typedef id MTLRenderPipelineState;
#endif

class MetalRenderer : public BaseRenderer
{
    std::unique_ptr<uint8_t[]> textureDecompressedMem;
    std::chrono::steady_clock::time_point current_time;
public:
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;
    virtual uint32_t getLayoutHandle(VertexType type) override;
    
    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    virtual void removeTexture(TextureHandle handle) override;
    virtual void updateGlobalSceneBuffer(glm::mat4& Projection, glm::mat4& View) override;
    virtual void updatePerModelData(glm::mat4& mat) override;
    virtual void renderGeom(Geometry& geom) override;
    
    virtual void beginFrame() override;
    virtual void presentFrame() override;
    
    void renderText(std::string& text, glm::vec2 pos);

    MTLDevicePtr device;
    MTLViewPtr mtkView;
    
    MetalRenderer();
    ~MetalRenderer();
    
    void initializeRenderEngine();
    void createDepthTexture();
    void createWarningTexture();
    void createFontTexture();
    MTLRenderPipelineState createRenderDescriptor(VertexLayout& attributes);
    void createRenderPipelines();
};
