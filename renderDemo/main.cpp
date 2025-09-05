#include "vulkanManager.h"

int main() {
    VulkanManager VKmanager;

    try 
    {
        VKmanager.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}