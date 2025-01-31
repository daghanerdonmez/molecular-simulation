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

class Receiver
{
private:
    glm::dvec3 position;
    double radius;
public:
    Receiver(glm::dvec3 position, double radius);
    bool hit(glm::dvec3 particlePosition);
    glm::dvec3 getPosition();
    double getRadius();
};

#endif /* receiver_hpp */
