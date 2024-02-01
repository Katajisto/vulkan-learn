//
// Created by katajisto on 1.2.2024.
//

#ifndef VKBOOTPROJ_VKINIT_H
#define VKBOOTPROJ_VKINIT_H
#include "vulkan/vulkan.h"

namespace vi {
    VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspectMask);

    VkSubmitInfo2 submit_info(VkCommandBufferSubmitInfo *cmd, VkSemaphoreSubmitInfo *signalSemaphoreInfo,
                              VkSemaphoreSubmitInfo *waitSemaphoreInfo);

    VkCommandBufferSubmitInfo command_buffer_submit_info(VkCommandBuffer cmd);

    VkSemaphoreSubmitInfo semaphore_submit_info(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);
}


#endif //VKBOOTPROJ_VKINIT_H
