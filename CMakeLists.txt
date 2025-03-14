cmake_minimum_required(VERSION 3.12)
project(Molecular_Simulation VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set up GLM (header-only library)
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Set up yaml-cpp
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.7.0
)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Enable testing" FORCE)
FetchContent_MakeAvailable(yaml-cpp)

# Set up GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
FetchContent_MakeAvailable(glfw)

# Set source files
file(GLOB_RECURSE SOURCES 
    "Molecular Simulation/*.cpp"
    "Molecular Simulation/*.hpp"
    "Molecular Simulation/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Molecular Simulation"
    "${CMAKE_CURRENT_SOURCE_DIR}/Molecular Simulation/GLHeaders"
    "${CMAKE_CURRENT_SOURCE_DIR}/Molecular Simulation/Math"
    "${CMAKE_CURRENT_SOURCE_DIR}/Molecular Simulation/Boundaries"
    "${CMAKE_CURRENT_SOURCE_DIR}/Molecular Simulation/Receivers"
    "${CMAKE_CURRENT_SOURCE_DIR}/Molecular Simulation/Shaders"
    "${CMAKE_CURRENT_SOURCE_DIR}/Molecular Simulation/Config"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glm
    yaml-cpp
    OpenGL::GL
    glfw
)

# Copy shader and configuration files to build directory
file(GLOB SHADER_FILES "Molecular Simulation/Shaders/*.glsl" "Molecular Simulation/Shaders/*.vert" "Molecular Simulation/Shaders/*.frag")
file(GLOB CONFIG_FILES "Molecular Simulation/Config/*.yaml" "Molecular Simulation/Config/*.json")

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(FILENAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_CURRENT_BINARY_DIR}/Shaders/${FILENAME} COPYONLY)
endforeach()

foreach(CONFIG_FILE ${CONFIG_FILES})
    get_filename_component(FILENAME ${CONFIG_FILE} NAME)
    configure_file(${CONFIG_FILE} ${CMAKE_CURRENT_BINARY_DIR}/Config/${FILENAME} COPYONLY)
endforeach()

# Create directories in build folder
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Shaders)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Config)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Output)
