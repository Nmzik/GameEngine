//
//  MetalRenderer.hpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#pragma once

#include <stdio.h>
#include "../GameRenderer.h"
#include "../YtdLoader.h"
#include <mutex>

#if defined(__OBJC__)
#import <CoreVideo/CoreVideo.h>
#import <GLKit/GLKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <QuartzCore/QuartzCore.h>
typedef id<MTLDevice> MTLDevicePtr;
typedef MTKView* MTLViewPtr;
#else
#include <objc/NSObjCRuntime.h>
#include <objc/objc.h>
typedef id MTLDevicePtr;
typedef id MTLViewPtr;
#endif

class MetalRenderer : public GameRenderer
{
    uint8_t* textureDecompressedMem;
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

    std::mutex bufferLoad;
    
    MetalRenderer();
    ~MetalRenderer();
    
    void initializeEngine();
};
