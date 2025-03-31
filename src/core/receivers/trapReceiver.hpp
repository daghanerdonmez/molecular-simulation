//
//  trapReceiver.hpp
//  molecular-simulation
//
//  Created by Dağhan Erdönmez on 31.03.2025.
//

#ifndef trapReceiver_hpp
#define trapReceiver_hpp

#include <stdio.h>
#include <src/core/receivers/receiver.hpp>
#include <src/math/coordinateSystemTransformations.hpp>
#include "glm/glm.hpp"

class TrapReceiver: public Receiver {
private:
    double radius;
    double length;
    double theta;
    double deltaTheta;
    double thickness;
public:
    TrapReceiver(glm::dvec3 position, double radius, double length, double theta, double deltaTheta, double thickness);
    bool hit(glm::dvec3 particlePosition) const override;
    double getLength() const;
    double getDeltaTheta() const;
};

inline double TrapReceiver::getLength() const {
    return length;
}

inline double TrapReceiver::getDeltaTheta() const {
    return deltaTheta;
}

#endif /* trapReceiver_hpp */
