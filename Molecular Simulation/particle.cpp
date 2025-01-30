//
//  particle.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "particle.hpp"

Particle::Particle(float x, float y, float z) : position(x,y,z)
{
    //std::cout << "Particle created" << std::endl;
}

void Particle::move(float dx, float dy, float dz)
{
    position += glm::vec3(dx,dy,dz);
}

glm::vec3 Particle::getPosition()
{
    return position;
}
