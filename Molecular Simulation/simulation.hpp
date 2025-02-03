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

class Simulation
{
private:
    std::vector<Particle> particles;
    std::vector<Receiver> receivers;
    int aliveParticleCount;
    int leftSimulationCount;
    int RightSimulationCount;
    Simulation* leftSimulations;
    Simulation* rightSimulations;
    
public:
    Simulation();
    void iterateSimulation(int iterationCount, int currentFrame);
    std::vector<glm::dvec3> getAliveParticlePositions();
    std::vector<Receiver> getReceivers();
    bool checkReceivedForParticle(const Particle& particle,const Receiver& receiver);
};


#endif /* simulation_hpp */
