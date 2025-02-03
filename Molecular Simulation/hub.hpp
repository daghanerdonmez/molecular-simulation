//
//  hub.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#ifndef hub_hpp
#define hub_hpp

#include <stdio.h>
#include "simulation.hpp"
#include <vector>
#include <random>

class Hub
{
private:
    std::vector<Simulation> connectingSimulations;
    double totalSquaredRadius = 0;
    std::vector<double> cumulativeProbabilities;
    
public:
    Hub();
    void addSimulation(Simulation);
    void simulateParticleTransaction();
    void Hub::initializeProbabilities();
};

#endif /* hub_hpp */
