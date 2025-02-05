//
//  connection.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 4.02.2025.
//

#ifndef connection_hpp
#define connection_hpp

#include "particle.hpp"

enum class Direction {
    LEFT,
    RIGHT
};

class Connection {
public:
    virtual void receiveParticle(Particle* particle, Direction direction) = 0; // Pure virtual function
    virtual ~Connection() = default; // Virtual destructor for proper cleanup
};

#endif /* connection_hpp */

