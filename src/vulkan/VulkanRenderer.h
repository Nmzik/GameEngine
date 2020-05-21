#pragma once
#include <optional>
#include <set>
#include <stack>

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

static const VkFormat s_attribType[] = {
	VK_FORMAT_R8G8B8A8_UINT,
	VK_FORMAT_R16G16_SFLOAT,
	VK_FORMAT_R32G32B32_SFLOAT
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

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;

    std::vector<VkPipeline> graphicsPipelines;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
	uint32_t imageIndex;

	VkPipeline examplePipeline;
	VkBuffer exampleVertexBuffer;
	VkDeviceMemory exampleVertexBufferMemory;
	VkBuffer exampleIndexBuffer;
	VkDeviceMemory exampleIndexBufferMemory;

	VkImage defaultImage;
	VkDeviceMemory defaultImageMemory;
	VkImageView defaultImageView;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

	//UBO
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	VkDescriptorSetLayout descriptorSetLayout;

	std::vector<VkBuffer> uboGlobalsBuffers;
	std::vector<VkDeviceMemory>	uboGlobalBuffersMemory;

	VkBuffer uboModel;
	VkDeviceMemory uboModelMemory;

	VkSampler textureSampler;
public:
    VulkanRenderer(NativeWindow* window);
    ~VulkanRenderer();

    std::vector<const char*> getRequiredExtensions();

	bool checkValidationLayerSupport();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT & createInfo);

	void setupDebugMessenger();

    //vulkan setup functions
    void createInstance();
    void createSurface();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void createSwapChain();
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createImageViews();
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage & image, VkDeviceMemory & imageMemory);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void createDefaultTexture();
    VkShaderModule createShaderModule(const std::vector<char>& code);
	void createExampleGraphicsPipeline();
	void createExampleVertexBuffer();
	void createExampleIndexBuffer();
	VkPipeline createGraphicsPipeline(VertexLayout& vertexLayout);
	void createDepthResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
    void createRenderPass();
    bool isDeviceSuitable(VkPhysicalDevice device);
    void pickPhysicalDevice();
    void createLogicalDevice();

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void createSyncObjects();

	void createDescriptorSetLayout();
	void createUniformBuffers();

	void createDescriptorPool();

	void createDescriptorSets();

    virtual TextureHandle createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format) override;
    virtual VertexBufferHandle createVertexBuffer(uint32_t size, const uint8_t* pointer) override;
    virtual IndexBufferHandle createIndexBuffer(uint32_t size, const uint8_t* pointer) override;
	void createTextureSampler();
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