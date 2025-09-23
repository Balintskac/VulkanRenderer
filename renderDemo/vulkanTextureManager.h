#pragma once


#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <stb_image.h>
#include "Core/vulkanDevice.h"
#include "BufferManager.h"

struct Texture {
	VkImage image;
	VkDeviceMemory memory;
	VkImageView imageView;
	VkSampler sampler;
};

class VulkanTextureManager 
{
private:
	VkDevice device;
	VkPhysicalDevice physicalDevice;

	int texWidth, texHeight, texChannels;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkCommandPool commandPool;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;


public:
	std::vector<Texture> textures; // 50 textúra Sponza-hoz
	static VkImageView textureImageView;
	static VkSampler textureSampler;
	static VkImageView depthImageView;

	VulkanTextureManager(const VkDevice& device,
		const VkPhysicalDevice& physicalDevice, 
		const VkCommandPool& commandPool) :
		device(device),
		physicalDevice(physicalDevice), 
		commandPool(commandPool){}

	void createTextureImage();

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void createTextureImageView();

	void createTextureSampler();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void copyBuffer(VkCommandBuffer& commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void transitionImageLayout(VkCommandBuffer& commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void copyBufferToImage(VkCommandBuffer& commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void createDepthResources(const VkExtent2D& swapChainExtent);

	VkFormat findSupportedFormat(
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features
	);

	VkFormat findDepthFormat();

	bool hasStencilComponent(VkFormat format);
};