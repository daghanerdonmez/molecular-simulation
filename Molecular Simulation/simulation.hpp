//
//  simulation.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#ifndef simulation_hpp
#define simulation_hpp

#include <stdio.h>
#include "particle.hpp"
#include "receiver.hpp"
#include <vector>
#include <glm/vec3.hpp>
#include "gaussian.hpp"
#include "Config/config.h"


void initializeSimulation();
void iterateSimulation(int iterationCount);
std::vector<glm::vec3> getParticlePositions();
std::vector<Receiver> getReceivers();
bool checkReceivedForParticle(Particle particle, Receiver receiver);

#endif /* simulation_hpp */
