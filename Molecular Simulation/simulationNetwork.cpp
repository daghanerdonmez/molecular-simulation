//
//  simulationNetwork.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#include "simulationNetwork.hpp"
#include <cstdlib> // For system()
#include <chrono>
#include <iomanip>
#include <sstream>

SimulationNetwork::SimulationNetwork()
{}

void SimulationNetwork::iterateNetwork(int iterationCount, int currentFrame)
{
    int localCurrentFrame = 0;
    
    for (int i = 0; i < iterationCount; ++i) {
        for(int j = 0; j < simulations.size(); ++j) {
            simulations[j]->iterateSimulation(1, currentFrame, i);
            if (localCurrentFrame % 1000 == 0) {
                std::cout << simulations[j]->getAliveParticleCount() << std::endl;
            }
        }
        if (localCurrentFrame % 1000 == 0) {
            std::cout << "--" << std::endl;
        }
        localCurrentFrame++;
    }
}

void SimulationNetwork::addSimulation(std::unique_ptr<Simulation> sim) {
    simulations.push_back(std::move(sim));  // moves ownership
}

void SimulationNetwork::addHub(std::unique_ptr<Hub> hub) {
    hubs.push_back(std::move(hub));
}

void SimulationNetwork::addSink(std::unique_ptr<Sink> sink) {
    sinks.push_back(std::move(sink));
}

void SimulationNetwork::simulationsWrite(const std::string &outputDir) const {
    // Create the output directory if it doesn't exist
    std::string mkdirCmd = "mkdir -p \"" + outputDir + "\"";
    system(mkdirCmd.c_str());
    
    // Create a timestamped directory for this run
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream timestamp;
    timestamp << std::put_time(std::localtime(&now_time_t), "%Y.%m.%d-%H.%M.%S");
    timestamp << "." << std::setfill('0') << std::setw(3) << now_ms.count();
    
    std::string runDir = outputDir + "/" + timestamp.str();
    std::string runDirCmd = "mkdir -p \"" + runDir + "\"";
    system(runDirCmd.c_str());
    
    // Have each simulation write its receivers' outputs to the timestamped subdirectory
    for (const auto& simulation: simulations) {
        simulation->receiversWrite(runDir);
    }
}

Simulation* SimulationNetwork::getFirstSimulation()
{
    return simulations[0].get();
}

Simulation* SimulationNetwork::getSecondSimulation()
{
    return simulations[1].get();
}
