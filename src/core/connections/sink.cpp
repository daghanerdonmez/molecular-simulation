//
//  sink.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 27.03.2025.
//

#include "sink.hpp"
#include <src/core/particle.hpp>

Sink::Sink() {}

void Sink::receiveParticle(Particle* particle, Direction direction, double overflow)
{
    // Sink just "kills" the particles it receives - no need to do anything with them
    // The particle pointer is managed elsewhere, so we don't delete it here
}

void Sink::setName(const std::string& sinkName)
{
    name = sinkName;
}

const std::string& Sink::getName() const
{
    return name;
}
