#pragma once

#include <vulkan/vulkan_core.h>

#include <glm/glm.hpp>
#include "BufferManager.h"
#include "Core/vulkanDevice.h"
#include <tiny_obj_loader.h>
#include <iostream>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


struct VertexModel {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex;
	int32_t texIndex;

	bool operator==(const VertexModel& other) const {
		return pos == other.pos && color == other.color && tex == other.tex;
	}
};

namespace std {
	template<> struct hash<VertexModel> {
		size_t operator()(VertexModel const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.tex) << 1);
		}
	};
}

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