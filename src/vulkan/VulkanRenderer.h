#pragma once
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan.h>
#include "..//NativeWindow.h"
#include "../BaseRenderer.h"

class NativeWindow;

class GameWorld;
class Camera;
class Shader;
//
class YdrLoader;
class CBuilding;
class CPed;
class CVehicle;

struct GlobalSceneData
{
    glm::mat4 projection;
    glm::mat4 view;
};

class VulkanRenderer : public BaseRenderer
{
    NativeWindow* nativeWindow;
	//
    GlobalSceneData sceneData;

	std::vector<VertexLayout> layouts;

	//
    VkSurfaceKHR surface;
	VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    uint32_t graphics_queue_node_index;
    std::vector<const char*> extensions;

    public:
    VulkanRenderer(NativeWindow* window);
    ~VulkanRenderer();

	//vulkan setup functions
    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();

    void createFramebuffers();

    void createCommandPool();

    void beginFrame();

    void endFrame();

    virtual void presentFrame() override;

    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;
    virtual uint32_t getLayoutHandle(VertexType type);

    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    virtual void removeTexture(TextureHandle handle) override;

    virtual void updateGlobalSceneBuffer(glm::mat4& Projection, glm::mat4& View) override;
    virtual void updatePerModelData(glm::mat4& mat) override;

	virtual void renderGeom(Geometry& geom) override;
};