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
    receivers.emplace_back(glm::vec3(RECEIVER_X, RECEIVER_Y, RECEIVER_Z), RECEIVER_RADIUS);
    
}

void iterateSimulation(int iterationCount)
{
    // for each iteration
    for(int i = 0; i < iterationCount; ++i) {
        // for each particle
        for(int j = 0; j < PARTICLE_COUNT; ++j) {
            // calculate their displacements
            // in brownian motion displacements in each iteration are standard normal distributions
            float dx = (float) generateGaussian(0.0, BROWNIAN_STD_DEV);
            float dy = (float) generateGaussian(0.0, BROWNIAN_STD_DEV);
            float dz = (float) generateGaussian(0.0, BROWNIAN_STD_DEV);
            particles[j].move(dx, dy, dz);
            
            //check if they are received by the receiver
            //TODO: I'm just sending these to another galaxy right now, not deleting them
            //TODO: But Ali Emre Pusane said it's okay :p
            if (checkReceivedForParticle(particles[j], receivers[0])) {
                particles[j].move(100000.0, 0.0, 0.0);
            }
        }
    }
}

bool checkReceivedForParticle(Particle particle, Receiver receiver)
{
    return receiver.hit(particle.getPosition());
}

std::vector<glm::vec3> getParticlePositions()
{
    //declare and initialize the positions vector with needed free slots
    std::vector<glm::vec3> positions;
    positions.reserve(PARTICLE_COUNT);
    
    //put each position in it's place by getting it from the object
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        positions.emplace_back(particles[i].getPosition());
    }
    
    return positions;
}

std::vector<Receiver> getReceivers()
{
    return receivers;
}
