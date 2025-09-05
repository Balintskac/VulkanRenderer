#include "vulkanManager.h"
#include "vulkanWindow.h"
#include "vulkanSwapChains.h"
#include "VulkanGraphicsPipeline.h"
#include "vulkanCommandBuffer.h"


void VulkanManager::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(1024, 1024, "Vulkan", nullptr, nullptr);
}

std::vector<const char*> VulkanManager::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

bool VulkanManager::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (layerFound = strcmp(layerName, layerProperties.layerName) == 0) {
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

VkApplicationInfo VulkanManager::createAppInfo()
{
    return  
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Test render",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };
}

void VulkanManager::createInstance()
{
    VkApplicationInfo appInfo = createAppInfo();

    auto extensions = getRequiredExtensions();
    /*
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        //    .pfnUserCallback = debugCallback
    };
    */
    createInstance(appInfo, validationLayers, extensions);
  //  createInstance(debugCreateInfo, appInfo, validationLayers, extensions);
}

void VulkanManager::createInstance(
    VkApplicationInfo appInfo,
    const std::vector<const char*> validationLayers,
    const std::vector<const char*> extensions
    )
{
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
         //   .pfnUserCallback = debugCallback
    };

    VkInstanceCreateInfo createInfo = {
       .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
   //    .pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo,
       .flags = 0,
       .pApplicationInfo = &appInfo,
       .enabledLayerCount = static_cast<uint32_t>(validationLayers.size()),
       .ppEnabledLayerNames = validationLayers.data(),
       .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
       .ppEnabledExtensionNames = extensions.data()
    };

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}

void VulkanManager::run()
{
    VulkanWindow vk;
    vk.createWindow();

    createInstance();
    vk.createSurface(instance);

    VulkanDevice vkd;
    vkd.pickPhysicalDevice(instance, vk.getSurface());
    vkd.createLogicalDevice(vk.getSurface());

    VulkanSwapChain vkSpawnChain(vkd.getDevice());

    vkSpawnChain.createSwapChain(vkd.getPhysicalDevice(), vk.getSurface(), &vk.getWindow());
    vkSpawnChain.createImageViews();

    VulkanGraphicsPipeline vkGraphicsPipeline;

    vkGraphicsPipeline.createRenderPass(vkd.getDevice(), vkSpawnChain.swapChainImageFormat);
    vkGraphicsPipeline.createGraphicsPipeline(vkd.getDevice());

    vkSpawnChain.createFramebuffers(vkGraphicsPipeline.renderPass);

    VulkanCommandBuffer vkCmdBuffer;

    auto queue = VulkanQueue::findQueueFamilies(vkd.getPhysicalDevice(), vk.getSurface());
    vkCmdBuffer.createCommandPool(vkd.getDevice(), queue);
    vkCmdBuffer.createCommandBuffer(vkd.getDevice());

    vkGraphicsPipeline.createSyncObjects(vkd.getDevice());

    while (!glfwWindowShouldClose(&vk.getWindow())) {
        glfwPollEvents();
        vkGraphicsPipeline.drawFrame(vkd.getDevice(), vkCmdBuffer, vkd.graphicsQueue, vkd.presentQueue,
            vkSpawnChain, vkGraphicsPipeline.imageAvailableSemaphore, vkGraphicsPipeline.renderFinishedSemaphore,
            vkGraphicsPipeline.renderPass);
    }

    vkDeviceWaitIdle(vkd.getDevice());

    cleanUp();
}

void VulkanManager::cleanUp()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
