cmake_minimum_required(VERSION 3.17)
project(nami)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps)

add_executable(nami
        src/main.cpp
        src/wave.cpp
        src/graphics/core.cpp
        src/graphics/generation.cpp
        deps/glad/glad.c
)

find_package(glfw3 REQUIRED)

target_link_libraries(nami ${GLFW_INCLUDE_DIRS} glfw)
