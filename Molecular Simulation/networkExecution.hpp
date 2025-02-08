//
//  networkExecution.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.02.2025.
//

#ifndef networkExecution_hpp
#define networkExecution_hpp

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
#include "simulationNetwork.hpp"
#include "GLHeaders/commonHeaders.hpp"
#include "simulationNetworkLoader.hpp"

int networkRunWithoutGraphics();
int networkRunWithGraphics();

#endif /* networkExecution_hpp */
