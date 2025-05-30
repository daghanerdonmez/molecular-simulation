//
//  particle.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "particle.hpp"
#include <src/core/connections/simulation.hpp>


Particle::Particle(double x, double y, double z)
    : position(x, y, z), alive(true), associatedBoundary(nullptr), associatedSimulation(nullptr) {}


void Particle::move(double dx, double dy, double dz, bool* toBeKilled)
{
    if (MODE == 0) {
        glm::dvec3 newPosition = position + glm::dvec3(dx,dy,dz);
        while (associatedBoundary->isOutsideBoundaries(newPosition)) {
            newPosition = associatedBoundary->reflectParticle(position, newPosition);
        }
        position = newPosition;
    } else if (MODE == 1) {
        glm::dvec3 newPosition = position + glm::dvec3(dx,dy,dz);
        
        if (!associatedBoundary) {
            throw std::runtime_error("associatedBoundary is null.");
        }
        if (!associatedSimulation) {
            throw std::runtime_error("associatedSimulation is null.");
        }
        // TODO: bunu her seferinde böyle kontrol etmek inefficient olabilir. belki de başta bir kere kontrol edip particle'ın bir özelliği olarak tutmam lazım particle initialize edildiği zaman
        Cylinder* cylinderBoundary = dynamic_cast<Cylinder*>(associatedBoundary);
        
        if (cylinderBoundary) {
            if (cylinderBoundary->isOutsideRightZBoundary(newPosition)) {
                if (associatedSimulation->getRightConnection() == nullptr) {
                    newPosition = associatedBoundary->reflectParticle(position, newPosition);
                } else {
                    double overflow = cylinderBoundary->getOverflow(newPosition);
                    associatedSimulation->giveParticleToRight(this, overflow);
                    *toBeKilled = true;
                    return;
                }
            } else if (cylinderBoundary->isOutsideLeftZBoundary(newPosition)) {
                if (associatedSimulation->getLeftConnection() == nullptr) {
                    newPosition = associatedBoundary->reflectParticle(position, newPosition);
                } else {
                    double overflow = cylinderBoundary->getOverflow(newPosition);
                    associatedSimulation->giveParticleToLeft(this, overflow);
                    *toBeKilled = true;
                    return;
                }
            } else {
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


