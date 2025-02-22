//
//  simulation.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "simulation.hpp"
#include "hub.hpp"

Simulation::~Simulation() {}

Simulation::Simulation(int particleCount, double radius, double length, glm::dvec3 flow) {
    if (MODE == 0) { // Single simulation
        this->flow = flow;
        aliveParticleCount = particleCount;
        
        if (SINGLE_APPLIED_BOUNDARY == 0) {
            boundary = std::make_unique<NoBoundary>();
        } else if (SINGLE_APPLIED_BOUNDARY == 1) {
            boundary = std::make_unique<Box>();
        } else if (SINGLE_APPLIED_BOUNDARY == 2) {
            boundary = std::make_unique<Cylinder>();
        }

        
        //initialize the particles vector with particleCount amount of free slots
        particles.reserve(particleCount);
        
        for (int i = 0; i < particleCount; ++i) {
            //this is an efficient method of constructing the particles in-place
            particles.emplace_back(0.0, 0.0, 0.0);
            particles[i].setBoundary(boundary.get());
            particles[i].setSimulation(this);
        }
        
        //initialize the receiver
        if (SINGLE_RECEIVER_COUNT != 0) {
            receivers.reserve(SINGLE_RECEIVER_COUNT);
            receivers.emplace_back(glm::dvec3(SINGLE_RECEIVER_X, SINGLE_RECEIVER_Y, SINGLE_RECEIVER_Z), SINGLE_RECEIVER_RADIUS);
        }
    } else if (MODE == 1) {
        this->flow = flow;
        aliveParticleCount = particleCount;
        boundary = std::make_unique<Cylinder>(radius, length);
        
        particles.reserve(particleCount);
        
        for (int i = 0; i < particleCount; ++i) {
            //this is an efficient method of constructing the particles in-place
            particles.emplace_back(0.0, 0.0, 0.0);
            particles[i].setBoundary(boundary.get());
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
                for(int j = 0; j < particles.size(); ++j) {
                    if (particles[j].isAlive()) {
                        // calculate their displacements
                        // in brownian motion displacements in each iteration are standard normal distributions
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.x * DT;
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.y * DT;
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.z * DT;
                        bool toBeKilled = false;
                        particles[j].move(dx, dy, dz, &toBeKilled);
                        
                        for (int k = 0; k < SINGLE_RECEIVER_COUNT; ++k) {
                            //check if they are received by the receivers
                            if (checkReceivedForParticle(particles[j], receivers[k])) {
                                killParticle(j);
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
                for(int j = 0; j < particles.size(); ++j) {
                    if (particles[j].isAlive()) {
                        // calculate their displacements
                        // in brownian motion displacements in each iteration are standard normal distributions
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.x * DT;
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.y * DT;
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.z * DT;
                        bool toBeKilled = false;
                        particles[j].move(dx, dy, dz, &toBeKilled);
                    }
                }
            }
        }
    } else if (MODE == 1) {
        // for each iteration
        for(int i = 0; i < iterationCount; ++i) {
            // for each particle
            for(int j = 0; j < particles.size(); ++j) {
                if (DEBUG_CHECKPOINT_PRINTS) {
                    printf("b1\n");
                }
                if (particles[j].isAlive()) {
                    if (DEBUG_CHECKPOINT_PRINTS) {
                        printf("b2\n");
                    }
                    // calculate their displacements
                    // in brownian motion displacements in each iteration are standard normal distributions
                    double dx = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.x * DT;
                    double dy = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.y * DT;
                    double dz = generateGaussian(0.0, sqrt(2 * D * DT)) + flow.z * DT;
                    bool toBeKilled = false;
                    particles[j].move(dx, dy, dz, &toBeKilled);
                    if (toBeKilled) {
                        killParticle(j);
                    }
                }
            }
        }
    }
}

bool Simulation::checkReceivedForParticle(const Particle& particle, const Receiver& receiver) const
{
    return receiver.hit(particle.getPosition());
}

std::vector<glm::dvec3> Simulation::getAliveParticlePositions() const
{
    //declare and initialize the positions vector with needed free slots
    std::vector<glm::dvec3> positions;
    positions.reserve(aliveParticleCount);
    
    //put each position in it's place by getting it from the object
    for (int i = 0; i < particles.size(); ++i) {
        if (particles[i].isAlive()) {
            positions.emplace_back(particles[i].getPosition());
        }
    }
    
    
    return positions;
}

double Simulation::getBoundaryRadius() const
{
    Cylinder* cylinderBoundary = dynamic_cast<Cylinder*>(boundary.get());
    
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

double Simulation::getBoundaryHeight() const
{
    Cylinder* cylinderBoundary = dynamic_cast<Cylinder*>(boundary.get());
    
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

void Simulation::giveParticleToLeft(Particle* particle, double overflow)
{
    Hub* hubConnection = dynamic_cast<Hub*>(leftConnection);
    if (hubConnection) {
        hubConnection->receiveParticle(particle, Direction::LEFT, overflow);
    } else {
        std::cerr << "Error: leftConnection is not a Hub or is nullptr." << std::endl;
    }
}

void Simulation::giveParticleToRight(Particle* particle, double overflow)
{
    Hub* hubConnection = dynamic_cast<Hub*>(rightConnection);
    if (hubConnection) {
        hubConnection->receiveParticle(particle, Direction::RIGHT, overflow);
    } else {
        std::cerr << "Error: rightConnection is not a Hub or is nullptr." << std::endl;
    }
}

void Simulation::receiveParticle(Particle* particle, Direction direction, double overflow)
{
    std::pair<double, double> xypair = generatePointInCircle(getBoundaryRadius());
    double zCoord = 0;
    if (direction == Direction::LEFT) {
        zCoord = -getBoundaryHeight() + overflow;
    } else if (direction == Direction::RIGHT) {
        zCoord = getBoundaryHeight() - overflow;
    }
    Particle newParticle(xypair.first, xypair.second, zCoord);
    newParticle.setBoundary(boundary.get());
    newParticle.setSimulation(this);
    addParticle(newParticle);
    aliveParticleCount++;
}

void Simulation::addParticle(const Particle& newParticle) {
    
    if (!inactiveIndices.empty()) {
        int index = inactiveIndices.top();
        inactiveIndices.pop();

        // Ensure index is within valid bounds before accessing the vector
        if (index >= 0 && index < particles.size()) {
            particles[index] = newParticle;
            particles[index].revive();
        } else {
            std::cerr << "!!!!!!!!!  Error: Retrieved invalid index from inactiveIndices. !!!!!!!!!!!!!\n";
        }
    } else {
        // when I remove this part the big bug that I'm facing occurs.
        // when the particles vector is empty and inactive indices is also empty
        // it gives an error while trying to allocate new space for the vector
        if (particles.capacity() == 0) {
            particles.reserve(100);  // Adjust based on expected usage
        }
        particles.push_back(newParticle);
    }
}


void Simulation::killParticle(int index) {
    if (index < 0 || index >= particles.size()) {
        std::cerr << "Error: Invalid particle index in killParticle\n";
        return;
    }

    if (!particles[index].isAlive()) {
        std::cerr << "Warning: Trying to kill an already dead particle.\n";
        return;
    }

    particles[index].kill();

    // Push the killed particle's index into inactiveIndices for reuse
    inactiveIndices.push(index);

    aliveParticleCount--;
}



