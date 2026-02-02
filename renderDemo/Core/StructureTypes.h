#pragma once
#include <vulkan/vulkan_core.h>
#include <glm/common.hpp>
#include <glm/mat4x4.hpp>
#include <array>

struct InstanceData {
    glm::vec3 offset;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static std::array<VkVertexInputBindingDescription, 1> getBindingDescription() {
        std::array<VkVertexInputBindingDescription, 1> bindingDescription = {};

        bindingDescription[0].binding = 0;
        bindingDescription[0].stride = sizeof(Vertex);
        bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        // 1. binding = instance
     //   bindingDescription[1].binding = 1;
     //   bindingDescription[1].stride = sizeof(InstanceData);
     //   bindingDescription[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

  //      attributeDescriptions[3].binding = 0;
  //      attributeDescriptions[3].location = 3;
   //     attributeDescriptions[3].format = VK_FORMAT_R32_SINT;
  //      attributeDescriptions[3].offset = offsetof(Vertex, texIndex);

        // instance offset
     //   attributeDescriptions[3].binding = 1;
     //   attributeDescriptions[3].location = 3;
     //   attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
     //   attributeDescriptions[3].offset = offsetof(InstanceData, offset);

        return attributeDescriptions;
    }
};

struct UniformBufferObject 
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct Camera {
    glm::vec3 position{ 0.0f, 0.0f, 10.0f };
    glm::vec3 front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    float yaw = -90.0f; // jobbra-balra forgás
    float pitch = 0.0f;   // fel-le forgás
    float speed = 10.5f;
    float sensitivity = 0.1f;
};

