cmake_minimum_required(VERSION 3.17)
project(nami)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${PROJECT_SOURCE_DIR}/includes)

add_subdirectory(lib/glfw-3.3.4)
add_subdirectory(lib/glm)

add_executable(nami
        src/main.cpp
        src/glad.c
)

target_link_libraries(nami glfw ${GLFW_LIBRARIES} glad glm)
