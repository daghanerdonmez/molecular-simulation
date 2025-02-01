//
//  simulation.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "simulation.hpp"

//declare a vector for particles
std::vector<Particle> particles;
std::vector<Receiver> receivers;

void initializeSimulation()
{
    //initialize the particles vector with particle_count amount of free slots
    particles.reserve(PARTICLE_COUNT);
    
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        //this is an efficient method of constructing the particles in-place
        particles.emplace_back(0.0, 0.0, 0.0);
    }
    
    //initialize the receiver
    receivers.reserve(RECEIVER_COUNT);
    receivers.emplace_back(glm::dvec3(RECEIVER_X, RECEIVER_Y, RECEIVER_Z), RECEIVER_RADIUS);
    
}

void iterateSimulation(int iterationCount)
{
    // for each iteration
    for(int i = 0; i < iterationCount; ++i) {
        // for each particle
        for(int j = 0; j < PARTICLE_COUNT; ++j) {
            if (particles[j].isAlive()) {
                for (int k = 0; k < RECEIVER_COUNT; ++k) {
                    // calculate their displacements
                    // in brownian motion displacements in each iteration are standard normal distributions
                    double dx = generateGaussian(0.0, BROWNIAN_STD_DEV);
                    double dy = generateGaussian(0.0, BROWNIAN_STD_DEV);
                    double dz = generateGaussian(0.0, BROWNIAN_STD_DEV);
                    particles[j].move(dx, dy, dz);
                    
                    //check if they are received by the receivers
                    //TODO: I'm just sending these to another galaxy right now, not deleting them
                    //TODO: But Ali Emre Pusane said it's okay :p
                    if (checkReceivedForParticle(particles[j], receivers[k])) {
                        particles[j].kill();
                    }
                }
            }
        }
    }
}

bool checkReceivedForParticle(Particle particle, Receiver receiver)
{
    return receiver.hit(particle.getPosition());
}

std::vector<glm::dvec3> getAliveParticlePositions()
{
    //declare and initialize the positions vector with needed free slots
    std::vector<glm::dvec3> positions;
    positions.reserve(PARTICLE_COUNT);
    
    //put each position in it's place by getting it from the object
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        if (particles[i].isAlive()) {
            positions.emplace_back(particles[i].getPosition());
        }
    }
    
    return positions;
}

std::vector<Receiver> getReceivers()
{
    return receivers;
}
