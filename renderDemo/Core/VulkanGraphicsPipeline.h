#pragma once

#include <vulkan/vulkan_core.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "vulkanCommandBuffer.h"
#include <stdexcept>
#include "vulkanWindow.h"
#include "structureTypes.h"

class VulkanGraphicsPipeline
{
private:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

public:
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
    VkRenderPass renderPass;

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

    void createGraphicsPipeline(const VkDevice& device);

    void createRenderPass(const VkDevice& device, const VkFormat& swapChainImageFormat);

    void createSyncObjects(const VkDevice& device);

    void drawFrame(const VulkanDevice& deviceManager, VulkanCommandBuffer& vkCmdBuffer, 
        VulkanSwapChain& swapChain, VulkanWindow& window, const VkBuffer& vertexBuffer);

    void recreateSwapChain(const VkDevice& device, const VulkanWindow& window);
  
    VkShaderModule createShaderModule(const VkDevice& device, const std::vector<char>& code);
};