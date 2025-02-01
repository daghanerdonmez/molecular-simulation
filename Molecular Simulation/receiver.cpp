//
//  receiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#include "receiver.hpp"

Receiver::Receiver(glm::dvec3 position, double radius): position(position), radius(radius)
{
    // initialize particles received to all 0
    for (int i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        particlesReceived[i] = 0;
    }
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

void Receiver::increaseParticlesReceived(int iterationNumber)
{
    particlesReceived[iterationNumber]++;
}

void Receiver::writeOutput()
{
    std::string output = "";
    
    for (int i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        output += std::to_string(particlesReceived[i]) + ",";
    }
    
    writeToFile("/Users/daghanerdonmez/Desktop/Molecular-Simulation/Molecular Simulation/Output/Outputs/r1output.txt", output);
}
