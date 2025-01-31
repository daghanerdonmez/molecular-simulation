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
    if (APPLIED_BOUNDARY == 0) { // no boundary
        position += glm::vec3(dx,dy,dz);
    } else if (APPLIED_BOUNDARY == 1) { // box boundary
        glm::vec3 newPosition = position + glm::vec3(dx,dy,dz);
        if (boxIsOutsideBoundaries(newPosition)) {
            newPosition = boxReflectParticle(newPosition);
        }
        position = newPosition;
    }
}

glm::vec3 Particle::getPosition()
{
    return position;
}
