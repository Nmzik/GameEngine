#pragma once
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan.h>
#include "..//NativeWindow.h"
#include "../BaseRenderer.h"

#include <optional>
#include <set>
#include <stack>

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

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
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

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    //swapChain stuff
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    public:
    VulkanRenderer(NativeWindow* window);
    ~VulkanRenderer();
    
    std::vector<const char*> getRequiredExtensions();

	//vulkan setup functions
    void createInstance();
    void createSurface();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void createSwapChain();
    void createImageViews();
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void createGraphicsPipeline();
    void createRenderPass();
    bool isDeviceSuitable(VkPhysicalDevice device);
    void pickPhysicalDevice();
    void createLogicalDevice();

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void createSyncObjects();

    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual uint32_t getLayoutHandle(VertexType type) override;

    virtual void removeTexture(TextureHandle handle) override;
    virtual void removeVertexBuffer(VertexBufferHandle handle) override;
    virtual void removeIndexbuffer(IndexBufferHandle handle) override;
    virtual void updateGlobalSceneBuffer(glm::mat4& Projection, glm::mat4& View) override;
    virtual void updatePerModelData(glm::mat4& mat) override;
    virtual void renderGeom(Geometry& geom) override;
    virtual void renderSky(Geometry& geom) override;
    /*void beginFrame();
     void endFrame();*/
    virtual void beginFrame() override;
    void createStagingBuffer();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    virtual void presentFrame() override;
};