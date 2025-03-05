//
//  tangentTrapReceiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.03.2025.
//

#include "tangentTrapReceiver.hpp"

bool tangentTrapReceiver::hit(glm::dvec3 particlePosition) const {
    glm::dvec3 cylParticlePos = cartesianToCylindrical(particlePosition);
    double rParticle = cylParticlePos.x;
    double thetaParticle = cylParticlePos.y;
    double zParticle = cylParticlePos.z;
    
    return (position.z - length/2 <= zParticle & zParticle <= position.z + length/2 &
            theta - deltaTheta/2 <= thetaParticle & thetaParticle <= theta + deltaTheta/2 &
            rParticle >= radius*0.95);
    //şimdilik r kontrolünü birazcık yanlış yapıyorum çünkü collision kontrolünü receiver kontrolünden önce yaptığım için particle'lar hiçbir zaman r > Radius olamaz. şimdilik böyle bi şey yaptım. bunu çözmenin bir yolu sanırım: receiver check'i particle.move'un içinde yapmak (collision check'ten önce). ama şimdi bir şeyleri bozmak istemiyorum sonra bakarım.
}
