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
#include <src/core/connections/simulation.hpp>
#include <src/config/config.h>
#include <src/config/oldconfig.h>
#include <src/core/connections/hub.hpp>
#include <src/core/connections/sink.hpp>

class SimulationNetwork
{
private:
    std::vector<std::unique_ptr<Simulation>> simulations;
    std::vector<std::unique_ptr<Hub>> hubs;
    std::vector<std::unique_ptr<Sink>> sinks;
public:
    SimulationNetwork();
    void iterateNetwork(int iterationCount, int currentFrame);
    void addSimulation(std::unique_ptr<Simulation> sim);
    void addHub(std::unique_ptr<Hub> hub);
    void addSink(std::unique_ptr<Sink> sink);
    
    void simulationsWrite(const std::string& outputDir) const;
    
    Simulation* getFirstSimulation();
    Simulation* getSecondSimulation();
};

#endif /* simulationNetwork_hpp */
