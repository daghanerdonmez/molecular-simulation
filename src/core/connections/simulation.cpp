//
//  simulation.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#include "simulation.hpp"
#include "hub.hpp"
#include <cstdlib> // For system()

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
//        particles.reserve(particleCount);
//        
//        for (int i = 0; i < particleCount; ++i) {
//            //this is an efficient method of constructing the particles in-place
//            particles.emplace_back(0.0, 0.0, 0.0);
//            particles[i].setBoundary(boundary.get());
//            particles[i].setSimulation(this);
//        }
        
        //initialize the receiver
        
        // TODO: THIS SHOULD CHANGE IT ONLY ADDS 1
        if (SINGLE_RECEIVER_COUNT != 0) {
            addReceiver(std::make_unique<SphericalReceiver>(glm::dvec3(SINGLE_RECEIVER_X, SINGLE_RECEIVER_Y, SINGLE_RECEIVER_Z),SINGLE_RECEIVER_RADIUS));
        }
    } else if (MODE == 1) {
        this->flow = flow;
        aliveParticleCount = particleCount;
        boundary = std::make_unique<Cylinder>(radius, length);
        
        particles.reserve(particleCount);
        
//        for (int i = 0; i < particleCount; ++i) {
//            //this is an efficient method of constructing the particles in-place
//            particles.emplace_back(0.0, 0.0, 0.0);
//            particles[i].setBoundary(boundary.get());
//            particles[i].setSimulation(this);
//        }
    }
}

void Simulation::iterateSimulation(int iterationCount, int currentFrame, int iterationInCurrentFrame)
{
    //std::cout << aliveParticleCount << std::endl;
    if (MODE == 0) {
        if (SINGLE_RECEIVER_COUNT != 0) {
            // for each iteration
            for(int i = 0; i < iterationCount; ++i) {
                
                for (auto& emitter : emitters) {
                    emitter->emit(currentFrame);
                }
                
                // for each particle
                for(int j = 0; j < particles.size(); ++j) {
                    if (particles[j].isAlive()) {
                        // calculate their displacements
                        // in brownian motion displacements in each iteration are standard normal distributions
                        glm::dvec3 particlePosition = particles[j].getPosition();
                        glm::dvec3 flowVector = getFlow(particlePosition);
                        
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.x * DT;
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.y * DT;
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.z * DT;
                        bool toBeKilled = false;
                        particles[j].move(dx, dy, dz, &toBeKilled);
                        
                        for (int k = 0; k < SINGLE_RECEIVER_COUNT; ++k) {
                            //check if they are received by the receivers
                            Receiver* receiver = receivers[k].get();
                            if (checkReceivedForParticle(particles[j], *receiver)) {
                                killParticle(j);
                                receiver->increaseParticlesReceived(currentFrame * ITERATIONS_PER_FRAME + i + iterationInCurrentFrame);
                                // I added iterationInCurrentFrame for the network simulation case but also added it here since its default value is 0
                            }
                        }
                    }
                }
            }
        } else {
            // for each iteration
            for(int i = 0; i < iterationCount; ++i) {
                
                for (auto& emitter : emitters) {
                    emitter->emit(currentFrame);
                }
                
                // for each particle
                for(int j = 0; j < particles.size(); ++j) {
                    if (particles[j].isAlive()) {
                        // calculate their displacements
                        // in brownian motion displacements in each iteration are standard normal distributions
                        glm::dvec3 particlePosition = particles[j].getPosition();
                        glm::dvec3 flowVector = getFlow(particlePosition);
                        
                        double dx = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.x * DT;
                        double dy = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.y * DT;
                        double dz = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.z * DT;
                        bool toBeKilled = false;
                        particles[j].move(dx, dy, dz, &toBeKilled);
                    }
                }
            }
        }
    } else if (MODE == 1) {
        // for each iteration
        for(int i = 0; i < iterationCount; ++i) {
            
            for (auto& emitter : emitters) {
                emitter->emit(currentFrame);
            }
            
            // for each particle
            for(int j = 0; j < particles.size(); ++j) {
                if (particles[j].isAlive()) {
                    // calculate their displacements
                    // in brownian motion displacements in each iteration are standard normal distributions
                    glm::dvec3 particlePosition = particles[j].getPosition();
                    glm::dvec3 flowVector = getFlow(particlePosition);
                    
                    double dx = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.x * DT;
                    double dy = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.y * DT;
                    double dz = generateGaussian(0.0, sqrt(2 * D * DT)) + flowVector.z * DT;
                    bool toBeKilled = false;
                    particles[j].move(dx, dy, dz, &toBeKilled);
                    
                    if (toBeKilled) {
                        killParticle(j);
                    } else {
                        for (int k = 0; k < receivers.size(); ++k) {
                            //check if they are received by the receivers
                            Receiver* receiver = receivers[k].get();
                            if (checkReceivedForParticle(particles[j], *receiver)) {
                                killParticle(j);
                                receiver->increaseParticlesReceived(currentFrame * ITERATIONS_PER_FRAME + i + iterationInCurrentFrame);
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Simulation::checkReceivedForParticle(const Particle& particle, const Receiver& receiver) const {
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
    if (!leftConnection) {
        std::cerr << "Error: leftConnection is nullptr." << std::endl;
        return;
    }
    
    leftConnection->receiveParticle(particle, Direction::LEFT, overflow);
}

void Simulation::giveParticleToRight(Particle* particle, double overflow)
{
    if (!rightConnection) {
        std::cerr << "Error: rightConnection is nullptr." << std::endl;
        return;
    }
    
    rightConnection->receiveParticle(particle, Direction::RIGHT, overflow);
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
    //aliveParticleCount++; //why the fuck is this not inside addParticle, im not going to change it because im afraid of breaking something. nvm changed it.
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
    
    aliveParticleCount++;
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

glm::dvec3 Simulation::getFlow(glm::dvec3 position) const{
    
    Cylinder* cylinderBoundary = dynamic_cast<Cylinder*>(boundary.get());
    
    // If boundary is of type Cylinder* the dynamic cast will return the pointer to the boundary
    // Else it will return nullptr and we are checking that below
    
    if (cylinderBoundary) {
        double x = position.x;
        double y = position.y;
        double z = position.z;
        double R = cylinderBoundary->getRadius();
        double rsquared;
        int cylinderOrientation = cylinderBoundary->getOrientation();
        if (cylinderOrientation == 0) {
            rsquared = (y*y + z*z);
        } else if(cylinderOrientation == 1){
            rsquared = (x*x + z*z);
        } else if(cylinderOrientation == 2){
            rsquared = (x*x + y*y);
        } else {
            throw std::runtime_error("Saçma error");
        }
        return flow * (1-(rsquared)/(R*R));
    } else {
        throw std::runtime_error("Boundary is not of type Cylinder.");
    }
}

void Simulation::receiversWrite(const std::string &baseDir) const {
    // Create a subdirectory for this simulation
    std::string simName = name.empty() ? "unnamed_simulation" : name;
    std::string simDir = baseDir + "/" + simName;
    
    // Create the directory if it doesn't exist
    std::string mkdirCmd = "mkdir -p \"" + simDir + "\"";
    system(mkdirCmd.c_str());
    
    // Write each receiver's output to a file in the simulation directory
    for (auto& receiver: receivers){
        if (dynamic_cast<SphericalReceiver*>(receiver.get())) {
            receiver->writeOutput(simDir, name, dynamic_cast<SphericalReceiver*>(receiver.get()), dynamic_cast<SphericalReceiver*>(receiver.get())->getRadius());
        } else {
            receiver->writeOutput(simDir, name, false, 0.0);
        }
    }
}

void Simulation::simulationDataWrite(const std::string &baseDir) const {
    // Create a subdirectory for this simulation
    std::string simName = name.empty() ? "unnamed_simulation" : name;
    std::string simDir = baseDir + "/" + simName;
    
    // Create the directory if it doesn't exist
    std::string mkdirCmd = "mkdir -p \"" + simDir + "\"";
    system(mkdirCmd.c_str());
    
    std::string output;
    output = name + " " + parentName + " " + std::to_string(getBoundaryHeight()) + " " + std::to_string(getBoundaryRadius()) + " " + std::to_string(receivers.size());
    
    // Write the output to a file
    std::string filename = simDir + "/simulation_data.txt";
    writeToFile(filename, output, false);
}
