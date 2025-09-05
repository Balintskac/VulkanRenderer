#pragma once
#define VULKAN_MANAGER

#include <vulkan/vulkan_core.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <stdexcept> 
#include <iostream>
#include "vulkanDevice.h"

class VulkanManager {
private:
	GLFWwindow* window;

	VkInstance instance;

	VkSurfaceKHR surface;

	VkDebugUtilsMessengerEXT debugMessenger;

	const bool enableValidationLayers = true;

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
public:
	VulkanManager() = default;

    VulkanManager(const VulkanManager&) = delete;
	VulkanManager& operator=(const VulkanManager&) = delete;

	VulkanManager operator=(const VulkanManager) = delete;

	VulkanManager(VulkanManager&&) = delete;
	VulkanManager& operator=(VulkanManager&&) = delete;

	void initWindow();
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	VkApplicationInfo createAppInfo();
	void createInstance();
	void createInstance(
		VkApplicationInfo appInfo,
		const std::vector<const char*> validationLayers,
		const std::vector<const char*> extensions);

	void run();

	void setupDebugMessenger();
	void cleanUp();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
};