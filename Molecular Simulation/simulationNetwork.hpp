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
    std::vector<std::unique_ptr<Hub>> hubs;
public:
    SimulationNetwork();
    void iterateNetwork(int iterationCount, int currentFrame);
    void addSimulation(std::unique_ptr<Simulation> sim);
    void addHub(std::unique_ptr<Hub> hub);
    
    void simulationsWrite(const std::string& path) const;
    
    Simulation* getFirstSimulation();
    Simulation* getSecondSimulation();
};

#endif /* simulationNetwork_hpp */
