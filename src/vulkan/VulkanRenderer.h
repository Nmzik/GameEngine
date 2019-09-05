#pragma once
#include "../GameRenderer.h"
#include <vulkan/vk_sdk_platform.h>
#include <vulkan/vulkan.h>

class GameWorld;
class Camera;
class Shader;
//
class YdrLoader;
class CBuilding;
class CPed;
class CVehicle;
class NativeWindow;

class VulkanRenderer : public GameRenderer
{
    NativeWindow* nativeWindow;
    VkInstance inst;

public:
    VulkanRenderer(NativeWindow* window);
    ~VulkanRenderer();

    /*void beginFrame();

    void endFrame();

    void presentFrame();

    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;

    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    virtual void removeTexture(TextureHandle handle) override;

    void renderDrawable(YdrLoader* drawable);
    void renderBuilding(CBuilding* building);
    void renderPed(CPed* ped);
    void renderVehicle(CVehicle* vehicle);
    //
    void renderWorld(GameWorld* world, Camera* curCamera) override;*/
};