#pragma once 

#include <vulkan/vulkan_core.h>
#include <iostream>
#include <vector>
#include "vulkanDevice.h"
#include <algorithm>
#include <GLFW/glfw3.h>

class VulkanSwapChain 
{

private:	
public:
	VkSwapchainKHR swapChain;
	VkDevice device;
	std::vector<VkImage> swapChainImages;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkFormat swapChainImageFormat;

	VulkanSwapChain(const VkDevice& device) : device(device){}

	~VulkanSwapChain() 
	{
		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}

		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(device, swapChain, nullptr);
	}

	void createSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface, GLFWwindow* window);
	void createImageViews();
	void createFramebuffers(VkRenderPass& renderPass);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

	VkImageView createImageView(VkImage image, VkFormat format);

};