#include "VulkanRenderer.h"
#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"

#include "../windows/Win32Window.h"

VulkanRenderer::VulkanRenderer(NativeWindow* window)
    : nativeWindow(window)
{
    //nativeWindow->initializeContext();
    createInstance();
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::createInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

	Win32Window* window = (Win32Window*)nativeWindow;

	uint32_t extensionCount;
    SDL_Vulkan_GetInstanceExtensions(window->getSDLWindow(), &extensionCount, NULL);
	extensions.reserve(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window->getSDLWindow(), &extensionCount, extensions.data());

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionCount);
    createInfo.ppEnabledExtensionNames = extensions.data();

    //VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    /*if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {*/
    createInfo.enabledLayerCount = 0;

    createInfo.pNext = nullptr;
    //}

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}

void VulkanRenderer::createSurface()
{
    Win32Window* window = (Win32Window*)nativeWindow;
    SDL_Vulkan_CreateSurface(window->getSDLWindow(), instance, &surface);
}

void VulkanRenderer::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    physicalDevice = devices[0];
    /*for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }*/

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

	uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    assert(queueFamilyCount >= 1);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);

	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    for (int i = 0; i < queueFamilyCount; i++)
    {
        if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (graphicsQueueNodeIndex == UINT32_MAX)
                graphicsQueueNodeIndex = i;
        }
    }
    graphics_queue_node_index = graphicsQueueNodeIndex;
}

void VulkanRenderer::createLogicalDevice()
{
    float queue_priorities[1] = {0.0};
    VkDeviceQueueCreateInfo queue = {};
    queue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue.pNext = NULL;
    queue.queueFamilyIndex = graphics_queue_node_index;
    queue.queueCount = 1;
    queue.pQueuePriorities = queue_priorities;

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = NULL;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queue;
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = NULL;
    deviceInfo.enabledExtensionCount = 0;
    deviceInfo.ppEnabledExtensionNames = (const char* const*)extensions.data();
    deviceInfo.pEnabledFeatures = NULL;

    if (vkCreateDevice(physicalDevice, &deviceInfo, NULL, &device) != VK_SUCCESS)
    {
		throw std::runtime_error("failed to create Device!");
	}
}

void VulkanRenderer::createFramebuffers()
{
    /*swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkImageView attachments[] = {
            swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }*/
}

void VulkanRenderer::createCommandPool()
{
    /*QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }*/
}

void VulkanRenderer::beginFrame()
{
}

void VulkanRenderer::endFrame()
{
}

void VulkanRenderer::presentFrame()
{
    nativeWindow->swapBuffers();
}

VertexBufferHandle VulkanRenderer::createVertexBuffer(uint32_t size, const uint8_t* pointer)
{
    return VertexBufferHandle{};
}

IndexBufferHandle VulkanRenderer::createIndexBuffer(uint32_t size, const uint8_t* pointer)
{
    return IndexBufferHandle{};
}

TextureHandle VulkanRenderer::createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format)
{
    /*unsigned int InternalFormat;
    unsigned int type;
    bool compressed = false;

    switch (format)
    {
        case D3DFMT_DXT1:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case D3DFMT_DXT3:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case D3DFMT_DXT5:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        case D3DFMT_BC7:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_BPTC_UNORM;
            break;
        case D3DFMT_ATI1:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RED_RGTC1;
            break;
        case D3DFMT_ATI2:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RG_RGTC2;
            break;
        case D3DFMT_A8R8G8B8:
            InternalFormat = GL_BGRA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        /*case D3DFMT_A1R5G5B5:  // NOTHING???!!!
            InternalFormat = GL_BGRA;
            type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
            break;
        case D3DFMT_A8:  // COLOR BLACK ALWAYS?
            //return;
            InternalFormat = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        case D3DFMT_A8B8G8R8:  // CANT Check!
            //return;
            InternalFormat = GL_RGBA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        case D3DFMT_L8:  // FONTS?
            //return;
            InternalFormat = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;*/
        /*default:
            return TextureHandle{0};
            printf("UNSUPPORTED FORMAT\n");
            break;
    }*/
    return TextureHandle{};
}

uint32_t VulkanRenderer::getLayoutHandle(VertexType type)
{
    auto iter = layoutHandles.find(type);
    if (iter != layoutHandles.end())
    {
        return iter->second;
    }
    else
    {
        printf("NOT FOUND\n");
        return 0;
    }
}

void VulkanRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
}

void VulkanRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
}

void VulkanRenderer::removeTexture(TextureHandle handle)
{
}

void VulkanRenderer::updateGlobalSceneBuffer(glm::mat4& Projection, glm::mat4& View)
{
}

void VulkanRenderer::updatePerModelData(glm::mat4& mat)
{
}

/*static const GLenum s_attribType[] =
    {
        GL_UNSIGNED_BYTE,  // Uint8
        GL_HALF_FLOAT,     // Half
        GL_FLOAT,          // Float
};*/

void VulkanRenderer::renderGeom(Geometry& geom)
{
}