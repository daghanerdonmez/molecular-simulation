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
                //std::cout << simulations[j]->getAliveParticleCount() << std::endl;
            }
        }
        if (localCurrentFrame % 1000 == 0) {
            //std::cout << "--" << std::endl;
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
    
    // Write simulation data for each simulation
    for (const auto& simulation: simulations) {
        simulation->simulationDataWrite(runDir);
    }

    // Write general data of the network (flow velocity, diffusion coefficient)
    std::string output;
    std::ostringstream oss;
    oss << std::setprecision(17) << D << " " << std::setprecision(17) << flow_value << "\n";
    output += oss.str();
    
    // Write the output to a file
    std::string filename = runDir + "/network_data.txt";
    writeToFile(filename, output, false);
    
    // Write the target output of the ML model
    filename = runDir + "/targetOutput.txt";
    output = "";
    for (const auto& simulation: simulations) {
        if (simulation->getEmitters().size() != 0) {
            output += simulation->getName();
            Emitter* emitter = simulation->getEmitters()[0].get();
            glm::dvec3 cylpos = cartesianToCylindrical(emitter->getPosition());
            output += " " + std::to_string(cylpos.x) + " " + std::to_string(cylpos.z) + "\n";
        }
    }
    writeToFile(filename, output, false);
}

Simulation* SimulationNetwork::getFirstSimulation()
{
    return simulations[0].get();
}

Simulation* SimulationNetwork::getSecondSimulation()
{
    return simulations[1].get();
}

int SimulationNetwork::getAliveParticleCountInNetwork()
{
    int count = 0;
    
    for (const auto& simulation: simulations) {
        count += simulation->getAliveParticleCount();
    }
    
    return count;
}

int SimulationNetwork::getParticlesInSinks()
{
    int count = 0;
    
    for (const auto& sink: sinks) {
        count += sink->getParticleCount();
    }
    
    return count;
}
