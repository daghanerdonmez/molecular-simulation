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
#include "Math/gaussian.hpp"
#include "Config/config.h"
#include "Boundaries/boundary.hpp"
#include <stdexcept>

class Simulation
{
private:
    std::vector<Particle> particles;
    std::vector<Receiver> receivers;
    int aliveParticleCount;
    Boundary* boundary = nullptr;
    
public:
    Simulation();
    void iterateSimulation(int iterationCount, int currentFrame);
    std::vector<glm::dvec3> getAliveParticlePositions();
    std::vector<Receiver> getReceivers();
    bool checkReceivedForParticle(const Particle& particle,const Receiver& receiver);
    double getBoundaryRadius(); // should only be called when boundary type is cylinder
};


#endif /* simulation_hpp */
