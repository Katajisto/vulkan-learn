#include "VkBootstrap.h"
#include <vulkan/vulkan.h>
#include <cmath>
#include <GLFW/glfw3.h>
#include "vkinit.h"

#include <iostream>


struct FrameData {
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkSemaphore swapchainSema, renderSema;
    VkFence renderFence;
};

constexpr  unsigned int FRAME_OVERLAP = 2;

class VulkanContext {
public:
    FrameData frames[FRAME_OVERLAP];
    FrameData& getCurrentFrame() { return frames[frameNumber % FRAME_OVERLAP];}
    int frameNumber;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;
    VkFormat swapchainImageFormat;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    VkExtent2D swapchainExtent;
    GLFWwindow *window;
    VkSwapchainKHR swapchain;
    VkQueue graphicsQueue;
    uint32_t graphicsQueueFamily;

    void init();
    void initVulkan();
    void initSwapchain();
    void initCommands();
    void initSyncStructures();
    void murderSwapchain();
    void muder();
    void doFrame();
};



void VulkanContext::initVulkan() {
    vkb::InstanceBuilder instance_builder;
    instance_builder.request_validation_layers ();
    instance_builder.use_default_debug_messenger();

    auto instance_builder_return = instance_builder.set_app_name("KTJST!").require_api_version(1,3,0).build();
    if (!instance_builder_return) {
        std::cerr << "Failed to create Vulkan instance. Error: " << instance_builder_return.error().message() << "\n";
    }

    vkb::Instance vkb_inst  = instance_builder_return.value();

    instance = vkb_inst.instance;
    debugMessenger = vkb_inst.debug_messenger;

    glfwCreateWindowSurface (instance, window, NULL, &surface);

    VkPhysicalDeviceVulkan12Features features{};
    VkPhysicalDeviceVulkan13Features feature2{};
    features.bufferDeviceAddress = true;
    features.descriptorIndexing = true;
    feature2.dynamicRendering = true;
    feature2.synchronization2 = true;

    vkb::PhysicalDeviceSelector selector{ vkb_inst };
    vkb::PhysicalDevice physicalDeviceTemp = selector
            .set_minimum_version(1, 3)
            .set_required_features_12(features)
            .set_required_features_13(feature2)
            .set_surface(surface)
            .select()
            .value();

    vkb::DeviceBuilder deviceBuilder{ physicalDeviceTemp };
    vkb::Device vkbDevice = deviceBuilder.build().value();

    device = vkbDevice.device;
    physicalDevice = physicalDeviceTemp.physical_device;

    graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

}

void VulkanContext::init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, false);
    window = glfwCreateWindow(1000, 1000, "KTJST!", NULL, NULL);

    initVulkan();
    initSwapchain();
    initCommands();
    initSyncStructures();
}

void VulkanContext::initSwapchain() {
    vkb::SwapchainBuilder swapchainBuilder{ physicalDevice,device,surface };

    swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

    vkb::Swapchain vkbSwapchain = swapchainBuilder
            //.use_default_format_selection()
            .set_desired_format(VkSurfaceFormatKHR{ .format = swapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
                    //use vsync present mode
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .set_desired_extent(1000, 1000)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build()
            .value();

    swapchainExtent = vkbSwapchain.extent;
    //store swapchain and its related images
    swapchain = vkbSwapchain.swapchain;
    swapchainImages = vkbSwapchain.get_images().value();
    swapchainImageViews = vkbSwapchain.get_image_views().value();
}

void VulkanContext::initCommands() {
    VkCommandPoolCreateInfo cmdpcreateinfo = {};
    cmdpcreateinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdpcreateinfo.pNext = nullptr;
    cmdpcreateinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdpcreateinfo.queueFamilyIndex = graphicsQueueFamily;

    for(int i = 0; i < FRAME_OVERLAP; i++) {
        vkCreateCommandPool(device, &cmdpcreateinfo, nullptr, &frames[i].commandPool);
        VkCommandBufferAllocateInfo allocinfo =  {};
        allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocinfo.pNext = nullptr;
        allocinfo.commandPool = frames[i].commandPool;
        allocinfo.commandBufferCount = 1;
        allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        vkAllocateCommandBuffers(device, &allocinfo, &frames[i].commandBuffer);
    }
}

void VulkanContext::initSyncStructures() {
    VkFenceCreateInfo fenceinfo = {};
    fenceinfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceinfo.pNext = nullptr;
    fenceinfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkSemaphoreCreateInfo semainfo = {};
    semainfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semainfo.pNext = nullptr;

    for(int i = 0; i < FRAME_OVERLAP; i++) {
        vkCreateFence(device, &fenceinfo, nullptr, &frames[i].renderFence);
        vkCreateSemaphore(device, &semainfo, nullptr, &frames[i].swapchainSema);
        vkCreateSemaphore(device, &semainfo, nullptr, &frames[i].renderSema);
    }
}

void VulkanContext::muder() {
    murderSwapchain();
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkb::destroy_debug_utils_messenger(instance, debugMessenger);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);

}

void VulkanContext::murderSwapchain() {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    for(int i = 0; i < swapchainImageViews.size(); i++) {
        vkDestroyImageView(device, swapchainImageViews[i], nullptr);
    }
}

void transitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout)
{
    VkImageMemoryBarrier2 imageBarrier {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
    imageBarrier.pNext = nullptr;

    imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
    imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

    imageBarrier.oldLayout = currentLayout;
    imageBarrier.newLayout = newLayout;

    VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    VkImageSubresourceRange subImage {};
    subImage.aspectMask = aspectMask;
    subImage.baseMipLevel = 0;
    subImage.levelCount = VK_REMAINING_MIP_LEVELS;
    subImage.baseArrayLayer = 0;
    subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

    imageBarrier.subresourceRange = subImage;
    imageBarrier.image = image;

    VkDependencyInfo depInfo {};
    depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    depInfo.pNext = nullptr;

    depInfo.imageMemoryBarrierCount = 1;
    depInfo.pImageMemoryBarriers = &imageBarrier;

    vkCmdPipelineBarrier2(cmd, &depInfo);
}

void VulkanContext::doFrame() {
    vkWaitForFences(device, 1, &getCurrentFrame().renderFence, true, 1000000000);
    vkResetFences(device, 1, &getCurrentFrame().renderFence);
    uint32_t swapchainImageIndex;
    vkAcquireNextImageKHR(device, swapchain, 1000000000, getCurrentFrame().swapchainSema, nullptr, &swapchainImageIndex);

    VkCommandBufferBeginInfo cmdbufinfo = {};
    cmdbufinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdbufinfo.pNext = nullptr;

    cmdbufinfo.pInheritanceInfo = nullptr;
    cmdbufinfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkCommandBuffer cmds = getCurrentFrame().commandBuffer;
    vkResetCommandBuffer(cmds, 0);
    vkBeginCommandBuffer(cmds, &cmdbufinfo);

    transitionImage(cmds, swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
    VkClearColorValue clearValue;
    float val1 = std::abs(std::sin(frameNumber / 120.f));
    float val2 = std::abs(std::sin(frameNumber / 60.f));
    float val3 = std::abs(std::cos(frameNumber / 900.f));

    clearValue = { { val1, val2, val3, 1.0f } };

    VkImageSubresourceRange clearRange = vi::image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);
    vkCmdClearColorImage(cmds, swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);
    transitionImage(cmds, swapchainImages[swapchainImageIndex],VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    vkEndCommandBuffer(cmds);

    VkCommandBufferSubmitInfo cmdinfo = vi::command_buffer_submit_info(cmds);

    VkSemaphoreSubmitInfo waitInfo = vi::semaphore_submit_info(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,getCurrentFrame().swapchainSema);
    VkSemaphoreSubmitInfo signalInfo = vi::semaphore_submit_info(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, getCurrentFrame().renderSema);

    VkSubmitInfo2 submit = vi::submit_info(&cmdinfo,&signalInfo,&waitInfo);


    vkQueueSubmit2(graphicsQueue, 1, &submit, getCurrentFrame().renderFence);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.swapchainCount = 1;

    presentInfo.pWaitSemaphores = &getCurrentFrame().renderSema;
    presentInfo.waitSemaphoreCount = 1;

    presentInfo.pImageIndices = &swapchainImageIndex;

    vkQueuePresentKHR(graphicsQueue, &presentInfo);

    frameNumber++;
}


int main() {

    VulkanContext vk{};
    vk.init();

    while(!glfwWindowShouldClose(vk.window)) {
        glfwPollEvents();
        vk.doFrame();
    }

    vk.muder();

    return 0;
}
