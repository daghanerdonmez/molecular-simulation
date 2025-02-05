//
//  simulation.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "simulation.hpp"
#include "hub.hpp"

Simulation::Simulation() {
    if (MODE == 0) { // Single simulation
        aliveParticleCount = PARTICLE_COUNT;
        
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
            particles[i].setSimulation(this);
        }
        
        //initialize the receiver
        if (SINGLE_RECEIVER_COUNT != 0) {
            receivers.reserve(SINGLE_RECEIVER_COUNT);
            receivers.emplace_back(glm::dvec3(SINGLE_RECEIVER_X, SINGLE_RECEIVER_Y, SINGLE_RECEIVER_Z), SINGLE_RECEIVER_RADIUS);
        }
    } else if (MODE == 1) {
        aliveParticleCount = PARTICLE_COUNT;
        boundary = new Cylinder();
        
        particles.reserve(PARTICLE_COUNT);
        
        for (int i = 0; i < PARTICLE_COUNT; ++i) {
            //this is an efficient method of constructing the particles in-place
            particles.emplace_back(0.0, 0.0, 0.0);
            particles[i].setBoundary(boundary);
            particles[i].setSimulation(this);
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
    } else if (MODE == 1) {
        // for each iteration
        for(int i = 0; i < iterationCount; ++i) {
            // for each particle
            for(int j = 0; j < PARTICLE_COUNT; ++j) {
                if (DEBUG_CHECKPOINT_PRINTS) {
                    printf("b1\n");
                }
                if (particles[j].isAlive()) {
                    if (DEBUG_CHECKPOINT_PRINTS) {
                        printf("b2\n");
                    }
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

double Simulation::getBoundaryRadius()
{
    Cylinder* cylinderBoundary = dynamic_cast<Cylinder*>(boundary);
    
    // If boundary is of type Cylinder* the dynamic cast will return the pointer to the boundary
    // Else it will return nullptr and we are checking that below
    // So basically we are returning the radius of the cylinder boundary if it is a cylinder
    // Else throwing an error, because we shouldn't have called this function
    
    if (cylinderBoundary) {
        return cylinderBoundary->getRadius();
    } else {
        throw std::runtime_error("Boundary is not of type Cylinder.");
    }
}

double Simulation::getBoundaryHeight()
{
    Cylinder* cylinderBoundary = dynamic_cast<Cylinder*>(boundary);
    
    // If boundary is of type Cylinder* the dynamic cast will return the pointer to the boundary
    // Else it will return nullptr and we are checking that below
    // So basically we are returning the height of the cylinder boundary if it is a cylinder
    // Else throwing an error, because we shouldn't have called this function
    
    if (cylinderBoundary) {
        return cylinderBoundary->getHeight();
    } else {
        throw std::runtime_error("Boundary is not of type Cylinder.");
    }
}

void Simulation::giveParticleToLeft(Particle* particle)
{
    Hub* leftHub = dynamic_cast<Hub*>(leftConnection);
    Direction left = Direction::LEFT; // this left is arbitrary direction is not used when giving particle from simulation to hub
    leftHub->receiveParticle(particle, left);
    particle->kill();
    aliveParticleCount--;
}

void Simulation::giveParticleToRight(Particle* particle)
{
    Hub* rightHub = dynamic_cast<Hub*>(leftConnection);
    Direction left = Direction::LEFT; // this left is arbitrary direction is not used when giving particle from simulation to hub
    rightHub->receiveParticle(particle, left);
    particle->kill();
    aliveParticleCount--;
}

void Simulation::receiveParticle(Particle* particle, Direction direction)
{
    std::pair<double, double> xypair = generatePointInCircle(getBoundaryRadius());
    double zCoord = 0;
    if (direction == Direction::LEFT) {
        zCoord = -getBoundaryHeight();
    } else if (direction == Direction::RIGHT) {
        zCoord = getBoundaryHeight();
    }
    Particle newParticle = Particle(xypair.first, xypair.second, zCoord);
    aliveParticleCount++;
    particles[aliveParticleCount] = newParticle;
}
