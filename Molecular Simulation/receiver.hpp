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
    int* particlesReceived;
    // Huge optimization: particles received used to be a static array like
    // int particlesReceived[NUMBER_OF_ITERATIONS]
    // That caused a huge problem, total complexity of the simulation was theta(n * log^4(n)) where n is NUMBER_OF_ITERATIONS
    // I think it was because a huge static array messed up with caching.
    // Now the complexity is theta(n) as expected
public:
    Receiver(glm::dvec3 position, double radius);
    bool hit(glm::dvec3 particlePosition) const;
    glm::dvec3 getPosition() const;
    double getRadius() const;
    void increaseParticlesReceived(int iterationNumber);
    void writeOutput();
};

#endif /* receiver_hpp */
