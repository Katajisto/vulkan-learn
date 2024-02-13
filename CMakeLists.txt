cmake_minimum_required(VERSION 3.27)
project(vkbootproj)

add_definitions(-DVK_KHR_synchronization2)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vk-bootstrap)

add_executable(vkbootproj main.cpp
        vkinit.cpp
        vkinit.h)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(vkbootproj Vulkan::Vulkan glfw vk-bootstrap::vk-bootstrap)
