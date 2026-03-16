#pragma once

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <imgui.h>

class VulkanContext
{
public:
    // Core Vulkan handles
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    uint32_t graphicsQueueFamily = 0;

    // ImGui integration
    VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;
    VkRenderPass imguiRenderPass = VK_NULL_HANDLE;
    int minImageCount = 2;
    int imageCount = 2;

    // For font upload
    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

    // Window + surface
    SDL_Window* window = nullptr;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VulkanContext(SDL_Window* sdlWindow);
    ~VulkanContext();

    void Init();
    void Cleanup();

    void BeginSingleTimeCommands();
    void EndSingleTimeCommands();

    void RenderImGuiDrawData(ImDrawData* drawData);
};
