//
//  trapReceiver.cpp
//  molecular-simulation
//
//  Created by Dağhan Erdönmez on 31.03.2025.
//

#include "trapReceiver.hpp"

TrapReceiver::TrapReceiver(glm::dvec3 position, double radius, double length, double theta, double deltaTheta, double thickness) : 
    Receiver(position), radius(radius), length(length), theta(theta), deltaTheta(deltaTheta), thickness(thickness) {}

bool TrapReceiver::hit(glm::dvec3 particlePosition) const {
    glm::dvec3 cylParticlePos = cartesianToCylindrical(particlePosition);
    double rParticle = cylParticlePos.x;
    double thetaParticle = cylParticlePos.y;
    double zParticle = cylParticlePos.z;
    
    return (position.z - length/2 <= zParticle & zParticle <= position.z + length/2 &
            theta - deltaTheta/2 <= thetaParticle & thetaParticle <= theta + deltaTheta/2 &
            rParticle >= radius-thickness);
}
