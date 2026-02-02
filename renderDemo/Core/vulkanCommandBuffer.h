#pragma once

#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include "vulkanQueue.h"
#include "vulkanSwapChains.h"
#include "../modelLoader.h"

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

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
        uint32_t imageIndex, VkRenderPass renderPass,
        VulkanSwapChain& vulkanSwapChain,
        VkPipeline graphicsPipeline,
        VkPipelineLayout& pipelineLayout,
        VkDescriptorSet& descriptorSets,
        const std::vector<MeshObject>& meshes);


    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
};