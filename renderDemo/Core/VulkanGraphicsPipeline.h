#pragma once

#include <vulkan/vulkan_core.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "vulkanCommandBuffer.h"
#include <stdexcept>
#include "vulkanWindow.h"
#include "structureTypes.h"
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VulkanGraphicsPipeline
{
private:
    VkPipeline graphicsPipeline;
    VkPhysicalDevice physicalDevice;

public:
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
    VkRenderPass renderPass;
    VkDevice device;
    Camera cam;

    VulkanGraphicsPipeline(const VkDevice& device,
        const VkPhysicalDevice& physicalDevice,
        Camera& cam)
        :  device(device), physicalDevice(physicalDevice), cam(cam){}

    ~VulkanGraphicsPipeline() 
    {
      //  vkDestroyPipeline(device, graphicsPipeline, nullptr);
      //  vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

     //   vkDestroyRenderPass(device, renderPass, nullptr);

    }
    
    std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    };

    void createGraphicsPipeline(VkPipelineLayout& pipelineLayout);

    void createRenderPass(const VkFormat& swapChainImageFormat);

    void createSyncObjects();

    void drawFrame(const VulkanDevice& deviceManager, VulkanCommandBuffer& vkCmdBuffer, 
        VulkanSwapChain& swapChain, VulkanWindow& window, void* uniformBuffersMapped, VkPipelineLayout& pipelineLayout,
        VkDescriptorSet& descriptorSets, const std::vector<MeshObject>& meshes);

    void recreateSwapChain(const VulkanWindow& window);
  
    VkShaderModule createShaderModule(const std::vector<char>& code);

    void updateUniformBuffer(void* uniformBuffersMapped, const uint32_t& width, const uint32_t& height);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
        VkImageTiling tiling, 
        VkFormatFeatureFlags features);

    VkFormat findDepthFormat();
};