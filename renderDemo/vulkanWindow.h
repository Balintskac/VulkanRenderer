#pragma once
#include "vulkan/vulkan_core.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

class VulkanWindow
{
    GLFWwindow* window;
    VkSurfaceKHR surface;
public:
    inline VkSurfaceKHR& getSurface() { return surface; }
    inline GLFWwindow& getWindow() { return *window; }

    void createSurface(VkInstance instance) 
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void createWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(1024, 1024, "Vulkan renderer", nullptr, nullptr);
    }
};