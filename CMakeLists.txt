cmake_minimum_required(VERSION 3.20)
project(craft-gl)
set(CMAKE_CXX_STANDARD 17)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME glfw
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.4
        OPTIONS
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_DOCS OFF"
        "GLFW_INSTALL OFF"
)

include_directories(include)
include_directories(vendor)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES} vendor/glad/glad.c)

target_link_libraries(${PROJECT_NAME} glfw)