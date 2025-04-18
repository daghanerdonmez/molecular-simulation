//
//  sink.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 27.03.2025.
//

#ifndef sink_hpp
#define sink_hpp

#include <stdio.h>
#include "connection.hpp"

class Sink: public Connection
{
private:
    std::string name;
    int particleCount;
    
public:
    Sink();
    ~Sink() override = default;
    
    // Implementation of Connection interface
    void receiveParticle(Particle* particle, Direction direction, double overflow) override;
    
    // Name getter and setter
    void setName(const std::string& sinkName);
    const std::string& getName() const;
    
    const int getParticleCount() const { return particleCount; }
};

#endif /* sink_hpp */
