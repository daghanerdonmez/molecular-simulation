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
    std::random_device rd;
    gen = std::mt19937(rd());
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

// Overload the output operator for Direction
// For debug purposes
// To be deleted
std::ostream& operator<<(std::ostream& os, Direction direction) {
    switch (direction) {
        case Direction::LEFT:
            os << "LEFT";
            break;
        case Direction::RIGHT:
            os << "RIGHT";
            break;
        default:
            os << "UNKNOWN";
    }
    return os;
}

void Hub::simulateParticleTransaction(Particle* particle, double overflow)
{
    std::uniform_real_distribution<> dis(0, totalSquaredRadius);
    double randomValue = dis(gen);
    
    for (size_t i = 0; i < cumulativeProbabilities.size(); ++i) {
            if (randomValue < cumulativeProbabilities[i]) {
                DirectedConnection dc = directedConnections[i];
                Simulation* sim = dynamic_cast<Simulation*>(dc.connection);
                sim->receiveParticle(particle, dc.direction, overflow);
                return;
            }
        }
}

// I am not actually using the particle pointer in any of the operations in this functions but I don't know whether it is a good idea to delete them. It causes no problem for now so I'll just leave it.
void Hub::receiveParticle(Particle* particle, Direction direction, double overflow)
{
    simulateParticleTransaction(particle, overflow);
}
