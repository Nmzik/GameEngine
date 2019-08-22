//
//  MetalRenderer.hpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#pragma once

#include <stdio.h>
#include <memory>
#include "../GameRenderer.h"
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

class MetalRenderer : public GameRenderer
{
    std::unique_ptr<uint8_t[]> textureDecompressedMem;
    std::chrono::steady_clock::time_point current_time;
public:
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;

    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    virtual void removeTexture(TextureHandle handle) override;

    virtual void renderWorld(GameWorld* world, Camera* curCamera) override;

    void renderDrawable(YdrLoader* drawable);
    void renderBuilding(CBuilding* building);
    void renderPed(CPed* ped);
    void renderVehicle(CVehicle* vehicle);

    MTLDevicePtr device;
    MTLViewPtr mtkView;
    
    MetalRenderer();
    ~MetalRenderer();
    
    void initializeRenderEngine();
    void createWarningTexture();
    MTLRenderPipelineState createRenderDescriptor(VertexLayout& attributes);
    void createRenderPipelines();
};
