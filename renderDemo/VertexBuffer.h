
#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <glm/common.hpp>
#include <stdexcept>

class VertexBuffer 
{
private:
    VkMemoryRequirements memRequirements;
    VkPhysicalDeviceMemoryProperties memProperties;
    VkBufferCreateInfo bufferInfo{};

public:
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer vertexBuffer;
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;
    };

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.2f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.5f}},
        {{0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
      0, 1, 2, 2, 3, 0
    };

	void createVertexBuffer(const VkDevice& device);
    void memoryAllocation(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);
    void fillVertexBuffer(const VkDevice& device);

    void createIndexBuffer(const VkDevice& device, const VkPhysicalDevice& physicalDevice);
};