//
//  simulation.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "simulation.hpp"
#include "hub.hpp"

Simulation::~Simulation() {
    delete boundary;
    boundary = nullptr;
}

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
            //std::cout << "Simulation instance address: " << this << std::endl;
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
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT));
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
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT));
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT));
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
                    //std::cout << "Simulation: " << this << " running iteration" << std::endl;
                    if (DEBUG_CHECKPOINT_PRINTS) {
                        printf("b2\n");
                    }
                    // calculate their displacements
                    // in brownian motion displacements in each iteration are standard normal distributions
                    double dx = generateGaussian(0.0, sqrt(2 * D * DT));
                    double dy = generateGaussian(0.0, sqrt(2 * D * DT));
                    double dz = generateGaussian(0.0, sqrt(2 * D * DT));
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
    for (int i = 0; i < particles.size(); ++i) {
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

int Simulation::getAliveParticleCount()
{
    return aliveParticleCount;
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
    //std::cout << "giving particle left" << std::endl;
    //std::cout << "çağrılan simülasyon: " << this << std::endl;
    Hub* hubConnection = dynamic_cast<Hub*>(leftConnection);
    if (hubConnection) {
        hubConnection->receiveParticle(particle, Direction::LEFT);
    } else {
        std::cerr << "Error: leftConnection is not a Hub or is nullptr." << std::endl;
    }
}

void Simulation::giveParticleToRight(Particle* particle)
{
    //std::cout << "giving particle right" << std::endl;
    //std::cout << "çağrılan simülasyon: " << this << std::endl;
    Hub* hubConnection = dynamic_cast<Hub*>(rightConnection);
    if (hubConnection) {
        hubConnection->receiveParticle(particle, Direction::RIGHT);
    } else {
        std::cerr << "Error: rightConnection is not a Hub or is nullptr." << std::endl;
    }
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
    Particle newParticle(xypair.first, xypair.second, zCoord);
    newParticle.setBoundary(boundary);
    newParticle.setSimulation(this);
    addParticle(newParticle);
    aliveParticleCount++;
}

void Simulation::setLeftConnection(Connection *connection)
{
    leftConnection = connection;
}

void Simulation::setRightConnection(Connection *connection)
{
    rightConnection = connection;
}

Connection* Simulation::getLeftConnection()
{
    return leftConnection;
}

Connection* Simulation::getRightConnection()
{
    return rightConnection;
}

void Simulation::addParticle(const Particle& newParticle) {
    //std::cout << "addParticleWorking: " << this << std::endl;
    
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

    //std::cout << "killParticleWorking: " << this << std::endl;
    particles[index].kill();

    // Push the killed particle's index into inactiveIndices for reuse
    inactiveIndices.push(index);

    aliveParticleCount--;
}



