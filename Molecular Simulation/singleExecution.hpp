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
#include "GLHeaders/commonHeaders.hpp"
#include "Shaders/shader.h"
#include <glm/glm.hpp>
#include <vector>
#include "Config/config.h"
#include "Config/oldconfig.h"
#include <time.h>

int singleRunWithGraphics();
int singleRunWithoutGraphics();

#endif /* singleExecution_hpp */
