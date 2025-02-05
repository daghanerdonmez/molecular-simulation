//
//  hub.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#include "hub.hpp"
#include "simulation.hpp"

Hub::Hub()
{
    
}

void Hub::addDirectedConnection(DirectedConnection directedConnection) {
    directedConnections.push_back(directedConnection);
}

void Hub::initializeProbabilities() {
    totalSquaredRadius = 0;
    cumulativeProbabilities.clear();

    for (DirectedConnection directedConnection : directedConnections) {
        // Assuming only Simulation connections have boundaries
        Simulation* sim = dynamic_cast<Simulation*>(directedConnection.connection);
        if (sim) {
            double radius = sim->getBoundaryRadius();
            totalSquaredRadius += radius * radius;
            cumulativeProbabilities.push_back(totalSquaredRadius);
        }
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

void Hub::receiveParticle(Particle* particle, Direction direction)
{
    simulateParticleTransaction();
}
