//
//  particle.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "particle.hpp"
#include "simulation.hpp"

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
    } else if (MODE == 1) {
        glm::dvec3 newPosition = position + glm::dvec3(dx,dy,dz);
        if (DEBUG_CHECKPOINT_PRINTS) {
            printf("c1\n");
        }
        
        // TODO: bunu her seferinde böyle kontrol etmek inefficient olabilir. belki de başta bir kere kontrol edip particle'ın bir özelliği olarak tutmam lazım particle initialize edildiği zaman
        Cylinder* cylinderBoundary = dynamic_cast<Cylinder*>(associatedBoundary);
        if (DEBUG_CHECKPOINT_PRINTS) {
            printf("c2\n");
        }
        
        if (cylinderBoundary) {
            if (DEBUG_CHECKPOINT_PRINTS) {
                printf("c3\n");
            }
            if (cylinderBoundary->isOutsideRightZBoundary(newPosition)) {
                if (DEBUG_CHECKPOINT_PRINTS) {
                    printf("c4\n");
                }
                printf("d");
                associatedSimulation->giveParticleToRight(this);
            } else if (cylinderBoundary->isOutsideLeftZBoundary(newPosition)) {
                if (DEBUG_CHECKPOINT_PRINTS) {
                    printf("c5\n");
                }
                associatedSimulation->giveParticleToLeft(this);
            } else {
                if (DEBUG_CHECKPOINT_PRINTS) {
                    printf("c6\n");
                }
                while (associatedBoundary->isOutsideBoundaries(newPosition)) {
                    newPosition = associatedBoundary->reflectParticle(position, newPosition);
                }
            }
        } else {
            throw std::runtime_error("Boundary is not of type Cylinder while moving.");
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

void Particle::setSimulation(Simulation *simulation)
{
    associatedSimulation = simulation;
}

void Particle::kill()
{
    alive = false;
}

bool Particle::isAlive()
{
    return alive;
}
