//
//  singleExecution.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#ifndef singleExecution_hpp
#define singleExecution_hpp

#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <iostream>
#include "particle.hpp"
#include "simulation.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include "Shaders/shader.h"
#include <glm/glm.hpp>
#include <vector>
#include "Config/config.h"
#include <time.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::vector<float> generateCircleVertices(float radius, int segments);
int singleRunWithGraphics();
int singleRunWithoutGraphics();

#endif /* singleExecution_hpp */
