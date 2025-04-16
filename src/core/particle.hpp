//
//  particle.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 29.01.2025.
//

#ifndef particle_hpp
#define particle_hpp

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <src/config/config.h>
#include <src/config/unused/oldconfig.h>
#include <src/core/boundaries/box.hpp>
#include <src/core/boundaries/cylinder.hpp>
#include <src/core/boundaries/noBoundary.hpp>
#include <src/core/boundaries/boundary.hpp>

class Simulation;

class Particle
{
private:
    glm::dvec3 position;
    bool alive;
    Boundary* associatedBoundary;
    Simulation* associatedSimulation;
    
public:
    Particle(double x, double y, double z);
    void move(double dx, double dy, double dz, bool* toBeKilled);
    const glm::dvec3& getPosition() const;
    void setBoundary(Boundary* boundary);
    void setSimulation(Simulation* simulation);
    void kill();
    void revive();
    bool isAlive() const;
};

inline const glm::dvec3& Particle::getPosition() const
{
    return position;
}

inline void Particle::setBoundary(Boundary* boundary)
{
    associatedBoundary = boundary;
}

inline void Particle::setSimulation(Simulation *simulation)
{
    associatedSimulation = simulation;
}

inline void Particle::kill()
{
    alive = false;
}

inline void Particle::revive(){
    alive = true;
}

inline bool Particle::isAlive() const
{
    return alive;
}

#endif /* particle_hpp */
