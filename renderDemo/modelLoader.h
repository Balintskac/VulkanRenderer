#pragma once

#include <vulkan/vulkan_core.h>

#include <glm/glm.hpp>
#include "BufferManager.h"
#include "Core/vulkanDevice.h"

#include <tiny_obj_loader.h>

struct VertexModel {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex;
};

class ModelLoader
{
private:
	std::vector<VertexModel> vertices;
	std::vector<uint32_t> indices;
	VkPhysicalDevice pyhsicalDevice;

public:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkDevice device;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	ModelLoader(const VkDevice& device, const VkPhysicalDevice& pyhsicalDevice) : device(device), pyhsicalDevice(pyhsicalDevice){}

	void loadModel();

	void createsVertexBuffer();

	void createsIndexBuffer();

};