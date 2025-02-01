//
//  receiver.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#ifndef receiver_hpp
#define receiver_hpp

#include <stdio.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "Config/config.h"
#include <string>
#include "Output/writer.hpp"

class Receiver
{
private:
    glm::dvec3 position;
    double radius;
    int particlesReceived[NUMBER_OF_ITERATIONS];
public:
    Receiver(glm::dvec3 position, double radius);
    bool hit(glm::dvec3 particlePosition);
    glm::dvec3 getPosition();
    double getRadius();
    void increaseParticlesReceived(int iterationNumber);
    void writeOutput();
};

#endif /* receiver_hpp */
