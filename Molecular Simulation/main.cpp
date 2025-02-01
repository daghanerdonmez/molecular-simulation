//
//  main.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include "particle.hpp"
#include "simulation.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include "Shaders/shader.h"
#include <glm/glm.hpp>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::vector<float> generateCircleVertices(float radius, int segments);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
    
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Particle Simulation", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Compile shaders
    Shader particleShader("/Users/daghanerdonmez/Desktop/Molecular-Simulation/Molecular Simulation/Shaders/vertexshader.txt", "/Users/daghanerdonmez/Desktop/Molecular-Simulation/Molecular Simulation/Shaders/fragmentshader.txt");
    
    // For transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Particle square vertices (centered at origin, unit size)
    float squareVertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Generate circle vertices
    const int circleSegments = 30;
    std::vector<float> circleVertices = generateCircleVertices(1.0f, circleSegments);

    unsigned int circleVAO, circleVBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    //Initialize the simulation
    initializeSimulation();
    float particleSize = 0.01f;
    
//    std::vector<glm::vec3> positions = getParticlePositions();
//    std::vector<Receiver> receivers = getReceivers();
//    Receiver receiver = receivers[0];
//    glm::vec3 receiverPosition = receiver.getPosition();
//    std::cout << glm::length(positions[0] - receiverPosition) << std::endl;
//    std::cout << receiver.getRadius() << std::endl;
    
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Iterate the simulation
        iterateSimulation(1);
        std::vector<glm::dvec3> positions = getAliveParticlePositions();
        
        //Draw the receiver
        std::vector<Receiver> receivers = getReceivers();
        Receiver receiver = receivers[0];
        glm::dvec3 receiverPosition = receiver.getPosition();
        float receiverRadius = receiver.getRadius();
        
        particleShader.setVec2("objectPos", glm::dvec2(receiverPosition.x, receiverPosition.y));
        particleShader.setFloat("objectSize", receiverRadius);
        particleShader.setVec3("objectColor", glm::dvec3(0.0, 0.7, 0.7)); // Blue for receiver

        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleSegments + 2);
        
        // Draw the particles
        particleShader.use();
        glBindVertexArray(VAO);
        
        for (int i = 0; i < PARTICLE_COUNT; ++i) {
            particleShader.setVec2("objectPos", glm::dvec2(positions[i].x, positions[i].y));
            particleShader.setFloat("objectSize", particleSize);
            particleShader.setVec3("objectColor", glm::dvec3(1.0, 0.5, 0.0));
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

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
