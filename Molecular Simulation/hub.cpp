//
//  hub.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#include "hub.hpp"

Hub::Hub()
{
    
}

void Hub::addSimulation(Simulation simulation) 
{
    connectingSimulations.push_back(simulation);
}

void Hub::initializeProbabilities()
{
    for (int i = 0; i < connectingSimulations.size(); ++i) {
        double radius = connectingSimulations[i].getBoundaryRadius();
        totalSquaredRadius += radius * radius;
        cumulativeProbabilities.push_back(totalSquaredRadius);
    }
}

void Hub::simulateParticleTransaction()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, totalSquaredRadius);
    double randomValue = dis(gen);
    
    for (size_t i = 0; i < cumulativeProbabilities.size(); ++i) {
            if (randomValue < cumulativeProbabilities[i]) {
                // Simulation i is selected
                // I will implement this part.
                break;
            }
        }
}
