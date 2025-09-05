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
    void createCommandPool(const VkDevice& device, const QueueFamilyIndices& queueFamilyIndices);

    void createCommandBuffer(const VkDevice& device);

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
        uint32_t imageIndex, VkRenderPass renderPass, VulkanSwapChain& VulkanSwapChain,
        VkPipeline graphicsPipeline);
};