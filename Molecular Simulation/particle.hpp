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
#include "Boundaries/Box.hpp"

class Particle
{
private:
    glm::vec3 position;
public:
    Particle(float x, float y, float z);
    void move(float dx, float dy, float dz);
    glm::vec3 getPosition();
};

#endif /* particle_hpp */
