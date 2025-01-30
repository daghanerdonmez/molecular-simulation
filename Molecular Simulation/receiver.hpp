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
    glm::vec3 position;
    float radius;
public:
    Receiver(glm::vec3 position, float radius);
    bool hit(glm::vec3 particlePosition);
    glm::vec3 getPosition();
    float getRadius();
};

#endif /* receiver_hpp */
