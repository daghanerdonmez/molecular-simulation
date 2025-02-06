//
//  simulationNetwork.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#ifndef simulationNetwork_hpp
#define simulationNetwork_hpp

#include <stdio.h>
#include <vector>
#include "simulation.hpp"
#include "Config/config.h"
#include "hub.hpp"

class SimulationNetwork
{
private:
    std::vector<std::unique_ptr<Simulation>> simulations;
    std::vector<Hub> hubs;
public:
    SimulationNetwork();
    void iterateNetwork(int iterationCount, int currentFrame);
    Simulation* getFirstSimulation();
};

#endif /* simulationNetwork_hpp */
