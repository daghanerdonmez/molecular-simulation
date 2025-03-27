//
//  Header.h
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.02.2025.
//

#ifndef commonHeaders_hpp
#define commonHeaders_hpp

// Platform-specific includes
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLFW/glfw3.h>
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#else // Linux and other platforms
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif

#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::vector<float> generateCircleVertices(float radius, int segments);

#endif /* commonHeaders_hpp */
