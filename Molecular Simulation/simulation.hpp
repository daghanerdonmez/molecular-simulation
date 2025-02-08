//
//  simulation.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#ifndef simulation_hpp
#define simulation_hpp

#include <stdio.h>
#include "particle.hpp"
#include "receiver.hpp"
#include <vector>
#include <stack>
#include <glm/vec3.hpp>
#include "Math/gaussian.hpp"
#include "Config/config.h"
#include "Boundaries/boundary.hpp"
#include <stdexcept>
#include "Math/random.hpp"
#include "connection.hpp"

class Particle;

class Simulation: public Connection
{
private:
    std::vector<Particle> particles;
    std::stack<int> inactiveIndices; // Indices of inactive particles
    std::vector<Receiver> receivers;
    int aliveParticleCount;
    std::unique_ptr<Boundary> boundary;
    //gpt bu connectionları düz pointer olarak tutma weakptr sharedptr falan kullan diyo da bence gerek yok.
    Connection* leftConnection = nullptr;
    Connection* rightConnection = nullptr;
    
public:
    ~Simulation();
    Simulation(int particleCount = PARTICLE_COUNT, double radius = SINGLE_CYLINDER_R, double length = SINGLE_CYLINDER_Z);
    void iterateSimulation(int iterationCount, int currentFrame);
    
    void addParticle(const Particle& addParticle);
    void killParticle(int index);
    
    std::vector<glm::dvec3> getAliveParticlePositions();
    int getAliveParticleCount();
    std::vector<Receiver> getReceivers();
    
    bool checkReceivedForParticle(const Particle& particle,const Receiver& receiver);
    
    double getBoundaryRadius(); // should only be called when boundary type is cylinder
    double getBoundaryHeight(); // should only be called when boundary type is cylinder
    
    void giveParticleToLeft(Particle* particle);
    void giveParticleToRight(Particle* particle);
    void receiveParticle(Particle* particle, Direction direction) override;
    
    void setLeftConnection(Connection* connection);
    void setRightConnection(Connection* connection);
    
    Connection* getLeftConnection();
    Connection* getRightConnection();
};


#endif /* simulation_hpp */
