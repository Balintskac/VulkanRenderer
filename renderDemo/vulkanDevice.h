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
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VulkanDevice() = default;
	void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	void createLogicalDevice(VkSurfaceKHR surface);

    inline VkDevice& getDevice()  { return device; }

	inline VkPhysicalDevice& getPhysicalDevice() { return physicalDevice; }

	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};