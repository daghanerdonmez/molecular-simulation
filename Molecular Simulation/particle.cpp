//
//  particle.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "sarticle.hpp"

Particle::Particle(double x, double y, double z) : position(x,y,z)
{
    //std::cout << "Particle created" << std::endl;
}

void Particle::move(double dx, double dy, double dz)
{
    if (MODE == 0) {
        glm::dvec3 newPosition = position + glm::dvec3(dx,dy,dz);
        while (associatedBoundary->isOutsideBoundaries(newPosition)) {
            newPosition = associatedBoundary->reflectParticle(position, newPosition);
        }
        position = newPosition;
    }
}

glm::dvec3 Particle::getPosition() const
{
    return position;
}

void Particle::setBoundary(Boundary* boundary)
{
    associatedBoundary = boundary;
}

void Particle::kill()
{
    alive = false;
}

bool Particle::isAlive()
{
    return alive;
}
