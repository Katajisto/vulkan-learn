//
// Created by Tuomas Katajisto on 14.2.2024.
//

#ifndef VKBOOTPROJ_VERTEX_H
#define VKBOOTPROJ_VERTEX_H

#include <vulkan/vulkan_core.h>
#include <array>

#include <glm/glm.hpp>
using namespace glm;

// Vertex buffer thingy
struct Vertex {
    vec<2, float, defaultp> pos;
    vec<3, float, defaultp> color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }
};

#endif //VKBOOTPROJ_VERTEX_H
