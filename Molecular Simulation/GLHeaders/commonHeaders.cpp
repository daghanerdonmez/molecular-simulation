//
//  commonHeaders.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.02.2025.
//

#include "commonHeaders.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::vector<float> generateCircleVertices(float radius, int segments) {
    std::vector<float> vertices;
    vertices.push_back(0.0f); // Center X
    vertices.push_back(0.0f); // Center Y

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
    }
    return vertices;
}
