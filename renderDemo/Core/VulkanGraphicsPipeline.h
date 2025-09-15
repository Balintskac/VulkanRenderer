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

public:
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
    VkRenderPass renderPass;
    VkDevice device;

    VulkanGraphicsPipeline(const VkDevice& device) : device(device){}

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
        VulkanSwapChain& swapChain, VulkanWindow& window, const VkBuffer& vertexBuffer,
        const VkBuffer& indexBuffer, void* uniformBuffersMapped, VkPipelineLayout& pipelineLayout,
        VkDescriptorSet& descriptorSets, const VkBuffer& instanceBuffer);

    void recreateSwapChain(const VulkanWindow& window);
  
    VkShaderModule createShaderModule(const std::vector<char>& code);

    void updateUniformBuffer(void* uniformBuffersMapped, const uint32_t& width, const uint32_t& height);
};