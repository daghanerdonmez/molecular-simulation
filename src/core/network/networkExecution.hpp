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
#include <src/core/particle.hpp>
#include <src/core/connections/simulation.hpp>
#include <src/gl-headers/commonHeaders.hpp>
#include <src/shaders/shader.h>
#include <glm/glm.hpp>
#include <vector>
#include <src/config/config.h>
#include <src/config/oldconfig.h>
#include <time.h>
#include <src/core/network/simulationNetwork.hpp>
#include <src/core/network/simulationNetworkLoader.hpp>

int networkRunWithoutGraphics();
int networkRunWithGraphics();

#endif /* networkExecution_hpp */
