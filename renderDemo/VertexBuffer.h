
#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <glm/common.hpp>
#include <glm/mat4x4.hpp>
#include <stdexcept>
#include "Core/StructureTypes.h"
#include "vulkanTextureManager.h"

class VertexBuffer 
{
private:
    VkMemoryRequirements memRequirements;
    VkPhysicalDeviceMemoryProperties memProperties;
    VkBufferCreateInfo bufferInfo{};

public:
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkDeviceMemory vertexBufferMemory;
    VkBuffer vertexBuffer;

    VkBuffer uniformBuffers;
    VkDeviceMemory uniformBuffersMemory;
    void* uniformBuffersMapped;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 tex;
    };

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.2f, 0.0f},{ 1.0f, 0.0f }},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}, { 0.0f, 1.0f }},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f }},

            { { -0.5f, -0.5f, -0.5f }, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    std::vector<InstanceData> instances;

    const std::vector<uint16_t> indices = {
      0, 1, 2, 2, 3, 0,
       4, 5, 6, 6, 7, 4
    };

    void createVertexBuffer(const VkDevice& device);
    void memoryAllocation(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);
    void fillVertexBuffer(const VkDevice& device);

    void createIndexBuffer(const VkDevice& device, const VkPhysicalDevice& physicalDevice);

    void createDescriptorSetLayout(const VkDevice& device);

    void createUniformBuffers(const VkDevice& device, const VkPhysicalDevice& physicalDevice);

    void createBuffer(const VkDevice& device,
        const VkPhysicalDevice& physicalDevice, 
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties, 
        VkBuffer& buffer, 
        VkDeviceMemory& bufferMemory);

    void createDescriptorPool(const VkDevice& device);
  
    void createDescriptorSets(const VkDevice& device);
};