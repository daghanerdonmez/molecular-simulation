//
//  particle.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "particle.hpp"

Particle::Particle(double x, double y, double z) : position(x,y,z)
{
    //std::cout << "Particle created" << std::endl;
}

void Particle::move(double dx, double dy, double dz)
{
    if (APPLIED_BOUNDARY == 0) { // no boundary
        position += glm::dvec3(dx,dy,dz);
    } else if (APPLIED_BOUNDARY == 1) { // box boundary
        glm::dvec3 newPosition = position + glm::dvec3(dx,dy,dz);
        if (boxIsOutsideBoundaries(newPosition)) {
            newPosition = boxReflectParticle(newPosition);
        }
        position = newPosition;
    } else if (APPLIED_BOUNDARY == 2) { // cylinder boundary
        glm::dvec3 newPosition = position + glm::dvec3(dx,dy,dz);
        while(cylinderIsOutsideBoundaries(newPosition)) {
            newPosition = cylinderReflectParticle(position, newPosition);
        }
        position = newPosition;
    }
}

glm::dvec3 Particle::getPosition()
{
    return position;
}
