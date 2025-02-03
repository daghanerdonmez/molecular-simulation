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
#include "Config/config.h"
#include "Boundaries/box.hpp"
#include "Boundaries/cylinder.hpp"
#include "Boundaries/noBoundary.hpp"
#include "Boundaries/boundary.hpp"

class Particle
{
private:
    glm::dvec3 position;
    bool alive = true;
    Boundary* associatedBoundary;
public:
    Particle(double x, double y, double z);
    void move(double dx, double dy, double dz);
    glm::dvec3 getPosition() const;
    void setBoundary(Boundary* boundary);
    void kill();
    bool isAlive();
};

#endif /* particle_hpp */
