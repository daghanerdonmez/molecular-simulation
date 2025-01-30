//
//  receiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#include "receiver.hpp"

Receiver::Receiver(glm::vec3 position, float radius): position(position), radius(radius)
{
    
};

bool Receiver::hit(glm::vec3 particlePosition)
{
    float len = glm::length(particlePosition - position);
    return (len < radius);
};

glm::vec3 Receiver::getPosition()
{
    return position;
}

float Receiver::getRadius()
{
    return radius;
}
