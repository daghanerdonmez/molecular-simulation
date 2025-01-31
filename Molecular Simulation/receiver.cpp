//
//  receiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#include "receiver.hpp"

Receiver::Receiver(glm::dvec3 position, double radius): position(position), radius(radius)
{
    
};

bool Receiver::hit(glm::dvec3 particlePosition)
{
    double len = glm::length(particlePosition - position);
    return (len < radius);
};

glm::dvec3 Receiver::getPosition()
{
    return position;
}

double Receiver::getRadius()
{
    return radius;
}
