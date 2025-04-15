//
//  simulation.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#ifndef simulation_hpp
#define simulation_hpp

#include <stdio.h>
#include <src/core/particle.hpp>
#include <src/core/receivers/receiver.hpp>
#include <src/core/emitters/emitter.hpp>
#include <src/core/receivers/sphericalReceiver.hpp>
#include <vector>
#include <stack>
#include <memory>
#include <glm/vec3.hpp>
#include <src/math/gaussian.hpp>
#include <src/config/config.h>
#include <src/config/oldconfig.h>
#include <src/core/boundaries/boundary.hpp>
#include <stdexcept>
#include <src/math/random.hpp>
#include <src/core/connections/connection.hpp>

class Particle;

class Simulation: public Connection
{
private:
    std::vector<Particle> particles;
    std::stack<int> inactiveIndices; // Indices of inactive particles
    std::vector<std::unique_ptr<Receiver>> receivers;
    std::vector<std::unique_ptr<Emitter>> emitters;
    int aliveParticleCount;
    std::unique_ptr<Boundary> boundary;
    //gpt bu connectionları düz pointer olarak tutma weakptr sharedptr falan kullan diyo da bence gerek yok.
    Connection* leftConnection = nullptr;
    Connection* rightConnection = nullptr;
    glm::dvec3 flow;
    std::string name; // Name of the simulation (e.g., pipe0, pipe1)
    std::string parentName; // Name of the parent simulation (e.g., pipe0, pipe1)
    
public:
    ~Simulation();
    Simulation(int particleCount = PARTICLE_COUNT, double radius = SINGLE_CYLINDER_R, double length = SINGLE_CYLINDER_Z, glm::dvec3 flow = glm::dvec3(SINGLE_FLOW_X, SINGLE_FLOW_Y, SINGLE_FLOW_Z));
    void iterateSimulation(int iterationCount, int currentFrame, int iterationInCurrentFrame = 0);
    
    void addParticle(const Particle& addParticle);
    void killParticle(int index);
    
    std::vector<glm::dvec3> getAliveParticlePositions() const;
    int getAliveParticleCount() const;
    const std::vector<std::unique_ptr<Receiver>>& getReceivers() const;
    void addReceiver(std::unique_ptr<Receiver> receiver);
    void receiversWrite(const std::string& path) const;
    void simulationDataWrite(const std::string& path) const;
    
    bool checkReceivedForParticle(const Particle& particle, const Receiver& receiver) const;

    double getBoundaryRadius() const; // should only be called when boundary type is cylinder
    double getBoundaryHeight() const; // should only be called when boundary type is cylinder
    
    void giveParticleToLeft(Particle* particle, double overflow);
    void giveParticleToRight(Particle* particle, double overflow);
    void receiveParticle(Particle* particle, Direction direction, double overflow) override;
    
    void setLeftConnection(Connection* connection);
    void setRightConnection(Connection* connection);
    Connection* getLeftConnection() const;
    Connection* getRightConnection() const;
    
    glm::dvec3 getFlow(glm::dvec3 position) const;
    
    void addEmitter(std::unique_ptr<Emitter> emitter);
    const std::vector<std::unique_ptr<Emitter>>& getEmitters() const;
    
    Boundary* getBoundary() const;
    
    // Name getter and setter
    void setName(const std::string& simulationName);
    const std::string& getName() const;
    void setParentName(const std::string& parentName);
    const std::string& getParentName() const;
};

inline const std::vector<std::unique_ptr<Receiver>>& Simulation::getReceivers() const { return receivers; }
inline void Simulation::addReceiver(std::unique_ptr<Receiver> receiver) { receivers.push_back(std::move(receiver)); }
inline int Simulation::getAliveParticleCount() const { return aliveParticleCount; }

inline void Simulation::setLeftConnection(Connection *connection){ leftConnection = connection; }
inline void Simulation::setRightConnection(Connection *connection){ rightConnection = connection; }
inline Connection* Simulation::getLeftConnection() const{ return leftConnection; }
inline Connection* Simulation::getRightConnection() const{ return rightConnection; }

inline Boundary* Simulation::getBoundary() const { return boundary.get(); }

inline void Simulation::addEmitter(std::unique_ptr<Emitter> emitter) {
    emitters.push_back(std::move(emitter));
}

inline const std::vector<std::unique_ptr<Emitter>>& Simulation::getEmitters() const {
    return emitters;
}

inline void Simulation::setName(const std::string& simulationName) {
    name = simulationName;
}

inline const std::string& Simulation::getName() const {
    return name;
}

inline void Simulation::setParentName(const std::string& parentName) {
    this->parentName = parentName;
}

inline const std::string& Simulation::getParentName() const {
    return parentName;
}
#endif /* simulation_hpp */
