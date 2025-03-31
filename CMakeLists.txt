cmake_minimum_required(VERSION 3.5)
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
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

# Platform-specific configurations
if(WIN32 OR UNIX AND NOT APPLE)
    # Add GLAD source file explicitly for Windows and Linux
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/glheaders/glad/glad.c")
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"  # Add project root for <molecular-simulation/...> style includes
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glm
    yaml-cpp
    OpenGL::GL
    glfw
)

# Set the working directory for the executable to be the project root directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    XCODE_GENERATE_SCHEME TRUE
    XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Platform-specific libraries
if(APPLE)
    # macOS-specific frameworks
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
elseif(WIN32)
    # Windows-specific libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE gdi32)
elseif(UNIX AND NOT APPLE)
    # Linux-specific libraries
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE X11 dl pthread)
endif()

# Copy shader and configuration files to build directory
file(GLOB SHADER_FILES "src/shaders/*.glsl" "src/shaders/*.vert" "src/shaders/*.frag" "src/shaders/*.txt")
file(GLOB CONFIG_FILES "src/config/*.yaml" "src/config/*.json")

# Create directories in build folder
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/config)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/outputs)

# For multi-config generators (like Visual Studio)
if(CMAKE_CONFIGURATION_TYPES)
    # Copy to each configuration directory
    foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TYPE}/shaders)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TYPE}/config)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TYPE}/output)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TYPE}/output/outputs)
        
        foreach(SHADER_FILE ${SHADER_FILES})
            get_filename_component(FILENAME ${SHADER_FILE} NAME)
            configure_file(${SHADER_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TYPE}/shaders/${FILENAME} COPYONLY)
        endforeach()
        
        foreach(CONFIG_FILE ${CONFIG_FILES})
            get_filename_component(FILENAME ${CONFIG_FILE} NAME)
            configure_file(${CONFIG_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TYPE}/config/${FILENAME} COPYONLY)
        endforeach()
    endforeach()
else()
    # For single-config generators
    foreach(SHADER_FILE ${SHADER_FILES})
        get_filename_component(FILENAME ${SHADER_FILE} NAME)
        configure_file(${SHADER_FILE} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${FILENAME} COPYONLY)
    endforeach()
    
    foreach(CONFIG_FILE ${CONFIG_FILES})
        get_filename_component(FILENAME ${CONFIG_FILE} NAME)
        configure_file(${CONFIG_FILE} ${CMAKE_CURRENT_BINARY_DIR}/config/${FILENAME} COPYONLY)
    endforeach()
endif()
