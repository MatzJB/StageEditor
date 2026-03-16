#pragma once

#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>

namespace gui
{
    // constant window size
    constexpr int WIDTH = 500;
    constexpr int HEIGHT = 300;

    // main loop control
    inline bool isRunning = true;

    // SDL window and Vulkan surface
    inline SDL_Window* window = nullptr;
    inline VkSurfaceKHR surface = VK_NULL_HANDLE;

    // Vulkan rendering context
    inline VkInstance instance = VK_NULL_HANDLE;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    inline VkDevice device = VK_NULL_HANDLE;
    inline VkQueue graphicsQueue = VK_NULL_HANDLE;
    inline VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    inline VkRenderPass renderPass = VK_NULL_HANDLE;

    // Extend with command buffers, framebuffers, etc. as needed

    // SDL Event
    inline SDL_Event event;

    // Initialization and cleanup
    bool Init();
    void Cleanup();

    // Rendering
    void BeginRender() noexcept;
    void EndRender() noexcept;
    void Render() noexcept;
}
