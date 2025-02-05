//
//  hub.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#ifndef hub_hpp
#define hub_hpp

#include <stdio.h>
#include <vector>
#include <random>
#include "connection.hpp"

struct DirectedConnection {
    Connection* connection;
    Direction direction;
};

class Hub: public Connection
{
private:
    std::vector<DirectedConnection> directedConnections;
    double totalSquaredRadius = 0;
    std::vector<double> cumulativeProbabilities;
    
public:
    Hub();
    void addDirectedConnection(DirectedConnection directedConnection);
    void simulateParticleTransaction();
    void initializeProbabilities();
    
    void receiveParticle(Particle* particle, Direction direction) override;
};

#endif /* hub_hpp */

