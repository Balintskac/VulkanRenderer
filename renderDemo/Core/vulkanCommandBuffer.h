#pragma once

#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include "vulkanQueue.h"
#include "vulkanSwapChains.h"

class VulkanCommandBuffer 
{
private:
public:
    VkCommandBuffer commandBuffer;
    VkCommandPool commandPool;
    VkDevice device;

    VulkanCommandBuffer(const VkDevice& device): device(device){}

    ~VulkanCommandBuffer()
    {
      //  vkDestroyCommandPool(device, commandPool, nullptr);

    }

    void createCommandPool(const QueueFamilyIndices& queueFamilyIndices);

    void createCommandBuffer();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, VulkanSwapChain& vulkanSwapChain, VkPipeline graphicsPipeline, const VkBuffer& vertexBuffer, const VkBuffer& indexBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSets, const VkBuffer& instanceBuffer);


    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
};