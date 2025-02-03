//
//  simulationNetwork.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#ifndef simulationNetwork_hpp
#define simulationNetwork_hpp

#include <stdio.h>
#include "simulation.hpp"
#include "Config/config.h"

class SimulationNetwork
{
private:
    Simulation* simulations;
public:
    SimulationNetwork();
};

#endif /* simulationNetwork_hpp */
