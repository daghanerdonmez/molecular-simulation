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
#include <src/core/particle.hpp>
#include <src/core/connections/simulation.hpp>
#include <src/gl-headers/commonHeaders.hpp>
#include <src/shaders/shader.h>
#include <glm/glm.hpp>
#include <vector>
#include <src/config/config.h>
#include <src/config/unused/oldconfig.h>
#include <time.h>

int singleRunWithGraphics();
int singleRunWithoutGraphics();

#endif /* singleExecution_hpp */
