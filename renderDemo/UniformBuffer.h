#pragma once
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <vector>
#include <glm/mat4x4.hpp>

class UniformBuffer
{
private:

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkBuffer uniformBuffers;
    VkDeviceMemory uniformBuffersMemory;
    void* uniformBuffersMapped;

};