//
//  tangentTrapReceiver.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.03.2025.
//

#ifndef tangentTrapReceiver_hpp
#define tangentTrapReceiver_hpp

#include <stdio.h>
#include "glm/glm.hpp"
#include "receiver.hpp"
#include <src/math/coordinateSystemTransformations.hpp>

class tangentTrapReceiver : public Receiver {
private:
    double radius;
    double length;
    double theta;
    double deltaTheta;
public:
    tangentTrapReceiver(glm::dvec3 position, double radius, double length, double theta, double deltaTheta);
    bool hit(glm::dvec3 particlePosition) const override;
    double getLength() const;
    double getDeltaTheta() const;
};

inline double tangentTrapReceiver::getLength() const {
    return length;
}

inline double tangentTrapReceiver::getDeltaTheta() const {
    return deltaTheta;
}

#endif /* tangentTrapReceiver_hpp */
