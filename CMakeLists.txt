cmake_minimum_required(VERSION 3.27)
project(vkbootproj)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        fetch_vk_bootstrap
        GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
        GIT_TAG        v1.3.276 #suggest using a tag so the library doesn't update whenever new commits are pushed to a branch
)
FetchContent_MakeAvailable(fetch_vk_bootstrap)

add_executable(vkbootproj main.cpp
        vkinit.cpp
        vkinit.h)

target_link_libraries(vkbootproj glfw vulkan)
target_link_libraries(vkbootproj vk-bootstrap::vk-bootstrap)
