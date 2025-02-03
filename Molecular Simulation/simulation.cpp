//
//  simulation.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "simulation.hpp"

Simulation::Simulation() {
    if (MODE == 0) { // Single simulation
        aliveParticleCount = PARTICLE_COUNT;
        
        Boundary* boundary = nullptr;
        if (SINGLE_APPLIED_BOUNDARY == 0) {
            boundary = new NoBoundary();
        } else if(SINGLE_APPLIED_BOUNDARY == 1) {
            boundary = new Box();
        } else if (SINGLE_APPLIED_BOUNDARY == 2) {
            boundary = new Cylinder();
        }
        
        //initialize the particles vector with particle_count amount of free slots
        particles.reserve(PARTICLE_COUNT);
        
        for (int i = 0; i < PARTICLE_COUNT; ++i) {
            //this is an efficient method of constructing the particles in-place
            particles.emplace_back(0.0, 0.0, 0.0);
            particles[i].setBoundary(boundary);
        }
        
        
        
        //initialize the receiver
        if (SINGLE_RECEIVER_COUNT != 0) {
            receivers.reserve(SINGLE_RECEIVER_COUNT);
            receivers.emplace_back(glm::dvec3(SINGLE_RECEIVER_X, SINGLE_RECEIVER_Y, SINGLE_RECEIVER_Z), SINGLE_RECEIVER_RADIUS);
        }
    }
}

void Simulation::iterateSimulation(int iterationCount, int currentFrame)
{
    if (MODE == 0) {
        if (SINGLE_RECEIVER_COUNT != 0) {
            // for each iteration
            for(int i = 0; i < iterationCount; ++i) {
                // for each particle
                for(int j = 0; j < PARTICLE_COUNT; ++j) {
                    if (particles[j].isAlive()) {
                        // calculate their displacements
                        // in brownian motion displacements in each iteration are standard normal distributions
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT));
                        particles[j].move(dx, dy, dz);
                        
                        for (int k = 0; k < SINGLE_RECEIVER_COUNT; ++k) {
                            //check if they are received by the receivers
                            if (checkReceivedForParticle(particles[j], receivers[k])) {
                                particles[j].kill();
                                aliveParticleCount--;
                                receivers[k].increaseParticlesReceived(currentFrame * ITERATIONS_PER_FRAME + i);
                            }
                        }
                    }
                }
            }
        } else {
            // for each iteration
            for(int i = 0; i < iterationCount; ++i) {
                // for each particle
                for(int j = 0; j < PARTICLE_COUNT; ++j) {
                    if (particles[j].isAlive()) {
                        // calculate their displacements
                        // in brownian motion displacements in each iteration are standard normal distributions
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT));
                        particles[j].move(dx, dy, dz);
                    }
                }
            }
        }
    }
}

bool Simulation::checkReceivedForParticle(const Particle& particle, const Receiver& receiver)
{
    return receiver.hit(particle.getPosition());
}

std::vector<glm::dvec3> Simulation::getAliveParticlePositions()
{
    //declare and initialize the positions vector with needed free slots
    std::vector<glm::dvec3> positions;
    positions.reserve(aliveParticleCount);
    
    //put each position in it's place by getting it from the object
    for (int i = 0; i < aliveParticleCount; ++i) {
        if (particles[i].isAlive()) {
            positions.emplace_back(particles[i].getPosition());
        }
    }
    
    return positions;
}

std::vector<Receiver> Simulation::getReceivers()
{
    return receivers;
}
