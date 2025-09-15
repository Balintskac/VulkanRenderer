#pragma once

#include <vulkan/vulkan_core.h>
#include "vulkanQueue.h"

class VulkanDevice 
{
private:
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;


	const std::vector<const char*> deviceExtensions = 
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
public:
	static VkQueue graphicsQueue;
	VkQueue presentQueue;
	VulkanDevice() = default;

	~VulkanDevice()
	{
		vkDestroyDevice(device, nullptr);
	}


	void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

	void createLogicalDevice(VkSurfaceKHR surface);

	static VkQueue& getGraphicsQueue(){ return graphicsQueue; }

    inline VkDevice& getDevice() const { return const_cast<VkDevice&>(device); }

	inline VkPhysicalDevice& getPhysicalDevice() const { return const_cast<VkPhysicalDevice&>(physicalDevice); }

	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};