#pragma once

#include <vulkan/vulkan_core.h>

#include <glm/glm.hpp>
#include "BufferManager.h"
#include "Core/vulkanDevice.h"
#include <tiny_obj_loader.h>
#include <iostream>
#include <unordered_map>
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/hash.hpp>

#include "vulkanTextureManager.h"

struct VertexModel {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex;
	//int32_t texIndex;

	/*
	bool operator==(const VertexModel& other) const {
		return pos == other.pos && color == other.color && tex == other.tex;
	}
	*/
};

struct Material {
	glm::vec3 diffuseColor;
	int diffuseTextureIndex; // index a textures vektorban
};

/*
namespace std {
	template<> struct hash<VertexModel> {
		size_t operator()(VertexModel const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.tex) << 1);
		}
	};
}
*/

struct VulkanTexture
{
	VkSampler sampler;
	VkImage image;
	VkImageLayout imageLayout;
	VkDeviceMemory deviceMemory;
	VkImageView view;
	uint32_t width, height;
	uint32_t mipLevels;
	uint32_t layerCount;
	VkDescriptorImageInfo descriptor;
};

struct ObjectMaterial
{
	std::string name;
	VkPipeline pipeline;
	// diffuse;
	Texture diffuse;
	Texture specular;
	Texture Ambient;
};

struct MeshObject {
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexMemory;

	uint32_t indexCount;
	uint32_t indexBase;

	VkDescriptorSet descriptorSet;

	ObjectMaterial* material;
};

class ModelLoader
{
private:
	std::vector<VertexModel> vertices;
	std::vector<uint32_t> indices;
	VkPhysicalDevice pyhsicalDevice;

	std::vector<Texture> textures;
	std::vector<Material> materials;
	std::unordered_map<std::string, int> textureLookup;

	std::vector<ObjectMaterial> objectMaterials;
	std::vector<VertexModel> gVertices;
	std::vector<uint32_t> gIndices;

public:
	std::vector<MeshObject> meshes;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkDevice device;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	ModelLoader(const VkDevice& device, const VkPhysicalDevice& pyhsicalDevice) : device(device), pyhsicalDevice(pyhsicalDevice){}

	void loadModel(VulkanTextureManager& vkTextureManager);

	void loadModelWithAssimp(VulkanTextureManager& vkTextureManager);

	void createsVertexBuffer(MeshObject& mesh, std::vector<VertexModel>& vertices);

	void createsIndexBuffer(MeshObject& mesh, std::vector<uint32_t>& indices);

	std::unique_ptr<Texture> loadTextureFromFile(const std::string& filename, VulkanTextureManager& vkTextureManager);

};