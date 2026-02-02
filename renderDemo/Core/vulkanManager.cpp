#include "vulkanManager.h"
#include "vulkanWindow.h"
#include "vulkanSwapChains.h"
#include "VulkanGraphicsPipeline.h"
#include "vulkanCommandBuffer.h"
#include "../VertexBuffer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../modelLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>



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
        .pEngineName = "R-Engine",
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
            .pfnUserCallback = VulkanManager::debugCallback
    };

    VkInstanceCreateInfo createInfo = {
       .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
       .pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo,
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

void VulkanManager::setupDebugMessenger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
 
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void VulkanManager::run()
{
    VulkanWindow vulkanWindow;
    vulkanWindow.createWindow();

   // glfwSetKeyCallback(window, keyCallback);
  //  glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowUserPointer(&vulkanWindow.getWindow(), &camera);
    glfwSetCursorPosCallback(&vulkanWindow.getWindow(), [](GLFWwindow * window, double xpos, double ypos) {
        // kiszedjük a user pointert és visszakasztoljuk
        Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));

        static float lastX = 400, lastY = 300;
        static bool firstMouse = true;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // y fordítva
        lastX = xpos;
        lastY = ypos;

        xoffset *= camera->sensitivity;
        yoffset *= camera->sensitivity;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f) camera->pitch = 89.0f;
        if (camera->pitch < -89.0f) camera->pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        direction.y = sin(glm::radians(camera->pitch));
        direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        camera->front = glm::normalize(direction);
    });
    // rejtse el az egeret és lockolja a közepére (FPS stílus)
    glfwSetInputMode(&vulkanWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    createInstance();
    vulkanWindow.createSurface(instance);

    setupDebugMessenger();

    VulkanDevice vkd;
    vkd.pickPhysicalDevice(instance, vulkanWindow.getSurface());
    vkd.createLogicalDevice(vulkanWindow.getSurface());

    auto& device = vkd.getDevice();

    VulkanSwapChain vkSpawnChain(device);

    vkSpawnChain.createSwapChain(vkd.getPhysicalDevice(), vulkanWindow.getSurface(), &vulkanWindow.getWindow());
    vkSpawnChain.createImageViews();

    VulkanGraphicsPipeline vkGraphicsPipeline(device, vkd.getPhysicalDevice(), camera);

    vkGraphicsPipeline.createRenderPass(vkSpawnChain.swapChainImageFormat);

    VulkanCommandBuffer vkCmdBuffer(device);
    auto queue = VulkanQueue::findQueueFamilies(vkd.getPhysicalDevice(), vulkanWindow.getSurface());
    vkCmdBuffer.createCommandPool(queue);
    vkCmdBuffer.createCommandBuffer();

    VulkanTextureManager textureManager(device, vkd.getPhysicalDevice(), vkCmdBuffer.commandPool);
    textureManager.createDepthResources(vkSpawnChain.swapChainExtent);
    textureManager.createTextureImage();
  //  textureManager.createTextureImageView();
   // textureManager.createTextureSampler();
   
    ModelLoader model(device, vkd.getPhysicalDevice());
    model.loadModelWithAssimp(textureManager);
 //   model.loadModel(textureManager);
   // model.createsVertexBuffer();
   // model.createsIndexBuffer();

    VertexBuffer vertexBuffer(model.meshes);

    vertexBuffer.createUniformBuffers(device, vkd.getPhysicalDevice());

    vertexBuffer.createDescriptorSetLayout(device);
    vkGraphicsPipeline.createGraphicsPipeline(vertexBuffer.pipelineLayout);

   // vertexBuffer.createVertexBuffer(device);
   // vertexBuffer.memoryAllocation(device, vkd.getPhysicalDevice());
  //  vertexBuffer.fillVertexBuffer(device);

//    vertexBuffer.createIndexBuffer(device, vkd.getPhysicalDevice());
    vertexBuffer.createDescriptorPool(device);
    vertexBuffer.createDescriptorSets(device, textureManager.textures, model.meshes);
    

    vkSpawnChain.createFramebuffers(vkGraphicsPipeline.renderPass);


    vkGraphicsPipeline.createSyncObjects();

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(&vulkanWindow.getWindow()))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();

        processInput(&vulkanWindow.getWindow(), deltaTime);

        vkGraphicsPipeline.cam.position = camera.position;
        vkGraphicsPipeline.cam.front = camera.front;

        vkGraphicsPipeline.drawFrame(vkd, vkCmdBuffer, vkSpawnChain, vulkanWindow,
           vertexBuffer.uniformBuffersMapped,
            vertexBuffer.pipelineLayout, vertexBuffer.descriptorSets, model.meshes);
    }

    vkDeviceWaitIdle(device);

    vkDestroyBuffer(device, vertexBuffer.indexBuffer, nullptr);
    vkFreeMemory(device, vertexBuffer.indexBufferMemory, nullptr);

    vkDestroyBuffer(device, vertexBuffer.vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBuffer.vertexBufferMemory, nullptr);
    cleanUp();
}

void VulkanManager::cleanUp()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

VkResult VulkanManager::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

    return VK_ERROR_EXTENSION_NOT_PRESENT;   
}

void VulkanManager::processInput(GLFWwindow* window, float deltaTime)
{
    float velocity = camera.speed * deltaTime;

//    std::cout << camera.position.x << std::endl;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += camera.front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= camera.front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * velocity;
}
