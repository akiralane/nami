cmake_minimum_required(VERSION 3.17)
project(nami)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(nami
        src/main.cpp
        src/graphics.cpp
        lib/glad/glad.c
)

find_package(glfw3 REQUIRED)

target_link_libraries(nami ${GLFW_INCLUDE_DIRS} glfw)
