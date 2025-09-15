#pragma once
#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <stb_image.h>
#include "Core/vulkanDevice.h"
#include "BufferManager.h"

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

public:
	static VkImageView textureImageView;
	static VkSampler textureSampler;

	VulkanTextureManager(const VkDevice& device,
		const VkPhysicalDevice& physicalDevice, const VkCommandPool& commandPool) : device(device), physicalDevice(physicalDevice), commandPool(commandPool){}

	void createTextureImage();

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void createTextureImageView();

	void createTextureSampler();

	VkImageView createImageView(VkImage image, VkFormat format);

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void copyBuffer(VkCommandBuffer& commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void transitionImageLayout(VkCommandBuffer& commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void copyBufferToImage(VkCommandBuffer& commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};