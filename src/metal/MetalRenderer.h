//
//  MetalRenderer.hpp
//  MyGameEngine
//
//  Created by nmzik on 16.08.2019.
//

#pragma once

#include <stdio.h>
#include "../GameRenderer.h"

#if defined(__OBJC__)
#  import <CoreVideo/CoreVideo.h>
#  import <QuartzCore/QuartzCore.h>
#  import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <GLKit/GLKit.h>
typedef id<MTLDevice> MTLDevicePtr;
typedef MTKView* MTLViewPtr;
#else
#  include <objc/objc.h>
#  include <objc/NSObjCRuntime.h>
typedef id MTLDevicePtr;
typedef id MTLViewPtr;
#endif

class MetalRenderer : public GameRenderer {
public:
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    
    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    
    virtual void renderWorld(GameWorld* world, Camera* curCamera) override;
    
    void renderDrawable(YdrLoader* drawable);
    void renderBuilding(CBuilding* building);
    void renderPed(CPed* ped);
    void renderVehicle(CVehicle* vehicle);
    
    MTLDevicePtr device;
    MTLViewPtr mtkView;
    
    MetalRenderer();
    ~MetalRenderer();
};
